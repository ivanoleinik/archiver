#include <iostream>
#include <queue>
#include <streambuf>
#include <string>
#include <memory>
#include "huffman.h"

TreeNode::TreeNode(unsigned char data, size_t freq) : data(data), freq(freq) {
    left = right = NULL;
}

TreeNode::TreeNode(size_t freq) : freq(freq) {
    left = right = NULL;
}

bool TreeNode::is_leaf() {
    return !left && !right;
}

bool CompareNodes::operator()(std::shared_ptr<TreeNode> left, std::shared_ptr<TreeNode> right) {
    return left->freq > right->freq;
}

void HuffTree::_fill_codes(std::shared_ptr<TreeNode> root, std::vector<bool> code) {
    if (!root)
        return;
    if (root->is_leaf())
        _codes[root->data] = code;

    auto push = [](std::vector<bool> v, bool x) { v.push_back(x); return v; };
    _fill_codes(root->left, push(code, 1));
    _fill_codes(root->right, push(code, 0));
}

HuffTree::HuffTree(std::vector<std::size_t> &frequencies) : _frequencies(frequencies) {
    for (size_t c = 0; c < 256; c++)
        _tree.push(std::make_shared<TreeNode>(c, _frequencies[static_cast<unsigned char>(c)]));

    std::shared_ptr<TreeNode> left, right, top;

    while (_tree.size() > 1) {
        left = _tree.top();
        _tree.pop();
        right = _tree.top();
        _tree.pop();

        top = std::make_shared<TreeNode>(left->freq + right->freq);
        top->left = left;
        top->right = right;
        _tree.push(top);
    }

    _codes.assign(256, {});
    _fill_codes(_tree.top(), {});
}

std::vector<std::size_t> &HuffTree::get_frequencies() {
    return _frequencies;
}

std::vector< std::vector<bool> > &HuffTree::get_codes() {
    return _codes;
}

std::shared_ptr<TreeNode> HuffTree::get_root() {
    return _tree.top();
}

void HuffEncode::_serialize(std::ostream &out, bool bit) {
    _buff ^= bit * (1 << _buff_fill);
    _buff_fill++;
    if (_buff_fill == 8) {
        out.write(reinterpret_cast<char*>(&_buff), sizeof(int8_t));
        _encoded_data_size++;
        _buff_fill = 0;
        _buff = 0;
    }
}

void HuffEncode::_clear_buff(std::ostream &out) {
    if (_buff_fill) {
        out.write(reinterpret_cast<char*>(&_buff), sizeof(int8_t));
        _encoded_data_size++;
        _buff_fill = 0;
        _buff = 0;
    }
}

HuffEncode::HuffEncode(std::istream &in) {
    in.seekg(0, std::ios::end);
    _data.reserve(in.tellg());
    in.seekg(0, std::ios::beg);
    _data.assign((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

    std::vector<std::size_t> frequencies(256, 0);
    for (unsigned char c : _data)
        frequencies[c]++;
    _tree = std::make_shared<HuffTree>(frequencies);
    _codes = _tree->get_codes();
}

void HuffEncode::write(std::ostream &out) {
    for (size_t freq : _tree->get_frequencies())
        out.write(reinterpret_cast<char*>(&freq), sizeof(size_t));

    for (unsigned char c : _data) {
        for (bool bit : _codes[c]) {
            _serialize(out, bit);
        }
    }
    _clear_buff(out);
}

void HuffEncode::write_info() {
    std::cout << _data.size() << std::endl;
    std::cout << _encoded_data_size << std::endl;
    std::cout << 256 * sizeof(size_t) << std::endl;
}

HuffDecode::HuffDecode(std::istream &in) {
    in.seekg(0, std::ios::beg);

    std::vector<std::size_t> frequencies;
    frequencies.assign(256, 0);
    for (size_t &freq : frequencies)
        in.read(reinterpret_cast<char*>(&freq), sizeof(size_t));
    _tree = std::make_shared<HuffTree>(frequencies);
    _codes = _tree->get_codes();

    in.seekg(0, std::ios::end);
    _encoded_data.reserve(in.tellg());
    in.seekg(256 * sizeof(size_t), std::ios::beg);
    _encoded_data.assign((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

    std::shared_ptr<TreeNode> root = _tree->get_root();
    std::shared_ptr<TreeNode> curr_node = root;

    size_t freq_sum = 0;
    for (size_t freq : _tree->get_frequencies())
        freq_sum += freq;
    for (unsigned char c : _encoded_data) {
        if (!freq_sum)
            break;
        for (size_t bit = 0; bit < 8 && freq_sum; bit++) {
            if (c & (1 << bit))
                curr_node = curr_node->left;
            else
                curr_node = curr_node->right;

            if (curr_node->is_leaf()) {
                _decoded_data += curr_node->data;
                freq_sum--;
                curr_node = root;
            }
        }
    }
}

void HuffDecode::write(std::ostream &out) {
    out << _decoded_data;
}

void HuffDecode::write_info() {
    std::cout << _encoded_data.size() << std::endl;
    std::cout << _decoded_data.size() << std::endl;
    std::cout << 256 * sizeof(size_t) << std::endl;
}
