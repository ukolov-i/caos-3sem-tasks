#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "test_utility.h"


NOT_OPTIMIZE double 
avx_sin(double x);


NOT_OPTIMIZE void 
test(double x) {
    double r1 = sin(x);
    double r2 = avx_sin(x);
    LOG_RESULT(EQUAL_DOUBLE(r1, r2), "   %lf\n   %lf %lf\n", x, r1, r2);
}

TEST(0) {
    START_TEST(0);

    test(0);
}

TEST(1) {
    START_TEST(1);

    test(1.0);    
    test(2.4);
}

TEST(2) {
    START_TEST(2);

    for (double i = 0; i < 2; i += 0.25) {
        test(M_PI * i);
    }
}

TEST(3) {
    START_TEST(3);

    test(M_PI * (122 / 4));
    test(M_PI * (42 / 4));
    test(M_PI * (-5 / 4));
}

TEST(4) {
    START_TEST(4);
    
    test(0.123456);
    test(0.001234);
    test(0.000123);
}

TEST(5) {
    START_TEST(5);

    for (double i = -1.75; i < 1.75; i += 0.125) {
        test(i);
    }
}

TEST(6) {
    START_TEST(6);

    for (double i = -2; i < 2; i += 1.0 / 6.0) {
        test(M_PI * i);
    }
}

NOT_OPTIMIZE void
run_tests() {
    LOG_STARTING_TESTS();

    test0();
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();

    LOG_FINISHED_TESTS();
}


NOT_OPTIMIZE int 
main() {
    run_tests();
    return 0;
}   