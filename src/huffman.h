#ifndef HW_03_HUFFMAN_H
#define HW_03_HUFFMAN_H

#include <queue>
#include <vector>
#include <iostream>
#include <memory>

class HuffException : public std::exception {
public:
    HuffException(std::string msg) : std::exception() { _msg = msg; }
    const char* what() const noexcept { return _msg.c_str(); }
private:
    std::string _msg;
};

struct TreeNode {
    unsigned char data;
    size_t freq;
    std::shared_ptr<TreeNode> left, right;

    TreeNode(unsigned char data, size_t freq);
    TreeNode(size_t freq);

    bool is_leaf();
};

struct CompareNodes {
    bool operator()(std::shared_ptr<TreeNode> left, std::shared_ptr<TreeNode> right);
};

class HuffTree {
private:
    std::vector<std::size_t> _frequencies;
    std::priority_queue<std::shared_ptr<TreeNode>, std::vector< std::shared_ptr<TreeNode> >, CompareNodes> _tree;
    std::vector< std::vector<bool> > _codes;

    void _fill_codes(std::shared_ptr<TreeNode> root, std::vector<bool> code);

public:
    HuffTree(std::vector<size_t> &frequencies);

    std::vector<size_t> &get_frequencies();
    std::vector< std::vector<bool> > &get_codes();
    std::shared_ptr<TreeNode> get_root();
};

class HuffEncode {
private:
    std::string _data;
    std::shared_ptr<HuffTree> _tree;
    std::vector< std::vector<bool> > _codes;
    size_t _encoded_data_size = 0;

    size_t _buff_fill = 0;
    int8_t _buff = 0;
    void _serialize(std::ostream &out, bool bit);
    void _clear_buff(std::ostream &out);
public:
    HuffEncode(std::istream &in);

    void write(std::ostream &out);
    void write_info();
};

class HuffDecode {
private:
    std::string _encoded_data, _decoded_data;
    std::shared_ptr<HuffTree> _tree;
    std::vector< std::vector<bool> > _codes;

public:
    HuffDecode(std::istream &in);

    void write(std::ostream &out);
    void write_info();
};

#endif
