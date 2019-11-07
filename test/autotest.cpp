#include <iostream>
#include "autotest.h"

void Test::check(bool expr, const char *func, const char *filename, int lineNum) {
    if (!expr)
        std::cout << "test failed: " << func << "() in " << filename << ":" << lineNum << std::endl;
}
