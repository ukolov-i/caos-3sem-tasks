#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "test_utility.h"


NOT_OPTIMIZE void
test(const int a, const int b) {
    WRITE_INPUT_SWEEP("%d %d", a, b);
    int buffer[1];
    run_arch();
    read_output_sweepi(buffer);

    LOG_RESULT(a + b == buffer[0], "   %d %d\n", a + b, buffer[0]);
}

TEST(0) {
    START_TEST(0);
    test(1, 1);
}

TEST(1) {
    START_TEST(1);

    test(100, 101);
    test(10001, -100001);
}

TEST(2) {
    START_TEST(2);

    int a[5];
    for (size_t i = 0; i < 5; ++i) {
        a[i] = i;
        test(a[i], 1);
    }
}

TEST(3) {
    START_TEST(3);

    int a = 1e9;
    int b = 1e9;
    test(a, b);
}

TEST(4) {
    START_TEST(4);

    int a[10];
    for (size_t i = 0; i < 10; ++i) {
        a[i] = i;
    }
    int b[10];
    for (size_t i = 0; i < 10; ++i) {
        b[i] = i;
        test(a[i], b[i]);
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

    LOG_FINISHED_TESTS();
}


NOT_OPTIMIZE int 
main(int argc, char **argv) {
    init_arch(argc, argv);
    run_tests();
    quit();
    return 0;
}