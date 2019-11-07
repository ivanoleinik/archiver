#ifndef HW_03_HUFFMAN_TEST_H
#define HW_03_HUFFMAN_TEST_H

#include "autotest.h"
#include "huffman.h"

class HuffTest : public Test {
public:
    void test_hufftree();

    void test_emtpy();
    void test_one_letter_text();
    void test_simple_text();
    void test_huge_text();
    void test_russian_text();
    void test_many_strings();
    void test_control_characters();
    void test_ascii_table();
    void test_binary();
    void test_huge_binary();

    void run_all_tests();
};

#endif
