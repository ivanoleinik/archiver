#ifndef HW_03_AUTOTEST_H
#define HW_03_AUTOTEST_H
#define DO_CHECK(expr) check(expr, __FUNCTION__, __FILE__, __LINE__)

class Test {
public:
    void check(bool expr, const char *func, const char *filename, int lineNum);
    virtual void run_all_tests() = 0;
};

#endif
