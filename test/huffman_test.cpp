#include <iostream>
#include <sstream>
#include "autotest.h"
#include "huffman_test.h"
#include "huffman.h"

void HuffTest::test_hufftree() {
    std::vector<size_t> freq(256, 0);
    freq['a'] = 2;
    freq['b'] = 1;

    HuffTree tree = HuffTree(freq);
    DO_CHECK(tree.get_frequencies() == freq);

    std::vector<bool> a_code = {0};
    std::vector<bool> b_code = {1, 0};
    DO_CHECK(tree.get_codes()['a'] == a_code);
    DO_CHECK(tree.get_codes()['b'] == b_code);
}

void HuffTest::test_emtpy() {
    std::string input_data, decoded_data;
    std::stringstream input, encoded, decoded;

    input_data = "";
    input << input_data;

    HuffEncode encoded_empty = HuffEncode(input);
    encoded_empty.write(encoded);

    encoded.seekg(0, std::ios::end);
    DO_CHECK(encoded.tellg() == 256 * sizeof(size_t));
    encoded.seekg(0, std::ios::beg);

    HuffDecode decoded_empty = HuffDecode(encoded);
    decoded_empty.write(decoded);

    decoded >> decoded_data;
    DO_CHECK(decoded_data == input_data);
}

void HuffTest::test_one_letter_text() {
    std::string input_data, decoded_data;
    std::stringstream input, encoded, decoded;

    input_data = "a";
    input << input_data;

    HuffEncode encoded_text = HuffEncode(input);
    encoded_text.write(encoded);

    encoded.seekg(0, std::ios::end);
    DO_CHECK(encoded.tellg() == 256 * sizeof(size_t) + 1);
    encoded.seekg(0, std::ios::beg);

    HuffDecode decoded_text = HuffDecode(encoded);
    decoded_text.write(decoded);

    decoded >> decoded_data;
    DO_CHECK(decoded_data == input_data);
}

void HuffTest::test_simple_text() {
    std::string input_data, decoded_data;
    std::stringstream input, encoded, decoded;

    input_data = "text";
    input << input_data;

    HuffEncode encoded_text = HuffEncode(input);
    encoded_text.write(encoded);

    encoded.seekg(0, std::ios::end);
    DO_CHECK(encoded.tellg() == 256 * sizeof(size_t) + 1);
    encoded.seekg(0, std::ios::beg);

    HuffDecode decoded_text = HuffDecode(encoded);
    decoded_text.write(decoded);

    decoded >> decoded_data;
    DO_CHECK(decoded_data == input_data);
}

void HuffTest::test_huge_text() {
    std::string input_data, decoded_data;
    std::stringstream input, encoded, decoded;

    input_data = "";
    for (size_t i = 0; i < 5 * 1024 * 1024; i++)
        input_data += static_cast<char>('a' + rand() % 26);
    input << input_data;

    HuffEncode encoded_text = HuffEncode(input);
    encoded_text.write(encoded);

    encoded.seekg(0, std::ios::end);
    DO_CHECK(encoded.tellg() == 256 * sizeof(size_t) + 3150188);
    encoded.seekg(0, std::ios::beg);

    HuffDecode decoded_text = HuffDecode(encoded);
    decoded_text.write(decoded);

    decoded >> decoded_data;
    DO_CHECK(decoded_data == input_data);
}

void HuffTest::test_russian_text() {
    std::string input_data, decoded_data;
    std::stringstream input, encoded, decoded;

    input_data = "КаК жЕ кАзЯвИт ЭтОт ТеКсТ";
    DO_CHECK(input_data.size() == 46);
    input.write(input_data.data(), 46);

    HuffEncode encoded_text = HuffEncode(input);
    encoded_text.write(encoded);

    encoded.seekg(0, std::ios::end);
    DO_CHECK(encoded.tellg() == 256 * sizeof(size_t) + 21);
    encoded.seekg(0, std::ios::beg);

    HuffDecode decoded_text = HuffDecode(encoded);
    decoded_text.write(decoded);

    decoded.seekg(0, std::ios::end);
    decoded_data.reserve(decoded.tellg());
    decoded.seekg(0, std::ios::beg);
    decoded_data.assign((std::istreambuf_iterator<char>(decoded)), std::istreambuf_iterator<char>());
    DO_CHECK(decoded_data == input_data);
}

void HuffTest::test_many_strings() {
    std::string input_data, decoded_data;
    std::stringstream input, encoded, decoded;

    input_data = "\n\n\na\nba\ncab\nada\nbac\nab\na\n\n\n";
    DO_CHECK(input_data.size() == 27);
    input.write(input_data.data(), 27);

    HuffEncode encoded_text = HuffEncode(input);
    encoded_text.write(encoded);

    encoded.seekg(0, std::ios::end);
    DO_CHECK(encoded.tellg() == 256 * sizeof(size_t) + 7);
    encoded.seekg(0, std::ios::beg);

    HuffDecode decoded_text = HuffDecode(encoded);
    decoded_text.write(decoded);

    decoded.seekg(0, std::ios::end);
    decoded_data.reserve(decoded.tellg());
    decoded.seekg(0, std::ios::beg);
    decoded_data.assign((std::istreambuf_iterator<char>(decoded)), std::istreambuf_iterator<char>());
    DO_CHECK(decoded_data == input_data);
}

void HuffTest::test_control_characters() {
    std::string input_data, decoded_data;
    std::stringstream input, encoded, decoded;

    input_data = "\n\v\b\f\t\t\f\a\t\v\r";
    DO_CHECK(input_data.size() == 11);
    input.write(input_data.data(), 11);

    HuffEncode encoded_binary = HuffEncode(input);
    encoded_binary.write(encoded);

    encoded.seekg(0, std::ios::end);
    DO_CHECK(encoded.tellg() == 256 * sizeof(size_t) + 4);
    encoded.seekg(0, std::ios::beg);

    HuffDecode decoded_binary = HuffDecode(encoded);
    decoded_binary.write(decoded);

    decoded.seekg(0, std::ios::end);
    decoded_data.reserve(decoded.tellg());
    decoded.seekg(0, std::ios::beg);
    decoded_data.assign((std::istreambuf_iterator<char>(decoded)), std::istreambuf_iterator<char>());
    DO_CHECK(decoded_data == input_data);
}

void HuffTest::test_ascii_table() {
    std::string input_data, decoded_data;
    std::stringstream input, encoded, decoded;

    input_data = "";
    for (size_t c = 0; c < 256; c++)
        input_data += static_cast<char>(c);
    DO_CHECK(input_data.size() == 256);
    input.write(input_data.data(), 256);

    HuffEncode encoded_binary = HuffEncode(input);
    encoded_binary.write(encoded);

    encoded.seekg(0, std::ios::end);
    DO_CHECK(encoded.tellg() == 256 * sizeof(size_t) + 256);
    encoded.seekg(0, std::ios::beg);

    HuffDecode decoded_binary = HuffDecode(encoded);
    decoded_binary.write(decoded);

    decoded.seekg(0, std::ios::end);
    decoded_data.reserve(decoded.tellg());
    decoded.seekg(0, std::ios::beg);
    decoded_data.assign((std::istreambuf_iterator<char>(decoded)), std::istreambuf_iterator<char>());
    DO_CHECK(decoded_data == input_data);
}

void HuffTest::test_binary() {
    std::string input_data, decoded_data;
    std::stringstream input, encoded, decoded;

    input_data = "";
    for (size_t i = 0; i < 100; i++)
        input_data += static_cast<char>(rand() % 256);
    DO_CHECK(input_data.size() == 100);
    input.write(input_data.data(), 100);

    HuffEncode encoded_binary = HuffEncode(input);
    encoded_binary.write(encoded);

    encoded.seekg(0, std::ios::end);
    DO_CHECK(encoded.tellg() == 256 * sizeof(size_t) + 79);
    encoded.seekg(0, std::ios::beg);

    HuffDecode decoded_binary = HuffDecode(encoded);
    decoded_binary.write(decoded);

    decoded.seekg(0, std::ios::end);
    decoded_data.reserve(decoded.tellg());
    decoded.seekg(0, std::ios::beg);
    decoded_data.assign((std::istreambuf_iterator<char>(decoded)), std::istreambuf_iterator<char>());
    DO_CHECK(decoded_data == input_data);
}

void HuffTest::test_huge_binary() {
    std::string input_data, decoded_data;
    std::stringstream input, encoded, decoded;

    input_data = "";
    for (size_t i = 0; i < 5 * 1024 * 1024; i++)
        input_data += static_cast<char>(rand() % 256);
    DO_CHECK(input_data.size() == 5 * 1024 * 1024);
    input.write(input_data.data(), 5 * 1024 * 1024);

    HuffEncode encoded_binary = HuffEncode(input);
    encoded_binary.write(encoded);

    encoded.seekg(0, std::ios::end);
    DO_CHECK(encoded.tellg() == 256 * sizeof(size_t) + 5 * 1024 * 1024);
    encoded.seekg(0, std::ios::beg);

    HuffDecode decoded_binary = HuffDecode(encoded);
    decoded_binary.write(decoded);

    decoded.seekg(0, std::ios::end);
    decoded_data.reserve(decoded.tellg());
    decoded.seekg(0, std::ios::beg);
    decoded_data.assign((std::istreambuf_iterator<char>(decoded)), std::istreambuf_iterator<char>());
    DO_CHECK(decoded_data == input_data);
}

void HuffTest::run_all_tests() {
    test_hufftree();

    test_emtpy();
    test_one_letter_text();
    test_simple_text();
    test_huge_text();
    test_russian_text();
    test_many_strings();
    test_control_characters();
    test_ascii_table();
    test_binary();
    test_huge_binary();
}
