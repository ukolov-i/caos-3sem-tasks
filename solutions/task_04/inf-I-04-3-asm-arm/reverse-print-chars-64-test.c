#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "test_utility.h"


NOT_OPTIMIZE char*
strrev(char* str) {
    for (char* front = str, * back = str + strlen(str) - 1; front < back; ++front, --back) {
        char c = *front;
        *front = *back;
        *back = c;
    }
    return str;
}


NOT_OPTIMIZE void
test(const char* data) {
    size_t len = strlen(data);
    char* copy = strdup(data);
    char buffer[1000];

    WRITE_INPUT_SWEEP("%s", data);
    run_arch();
    read_output_sweeps(buffer);

    int status = strcmp(strrev(copy), buffer);
    LOG_CONDITION(status == 0, "PASSED", "FAILED", "   %s %s\n", copy, buffer);
    free(copy);
}

TEST(0) {
    START_TEST(0);

    char a[2] = "a";
    test(a);
}

TEST(1) {
    START_TEST(1);

    char a[4] = "abc";
    test(a);
    char b[13] = "Hello world!";
    test(b);
}

TEST(2) {
    START_TEST(2);

    char a[100];
    strcpy(a, "a");
    for (size_t i = 0; i < 40; ++i) {
        strcat(a, "bc");
    }
    test(a);
    strcat(a, "cba");
    test(a);
}

TEST(3) {
    START_TEST(3);

    char a[100];
    strcpy(a, "/\n/");
    test(a);
}

TEST(4) {
    START_TEST(4);

    char a[600];
    strcpy(a, "Hello_world!");
    for (size_t i = 0; i < 140; ++i) {
        strcat(a, "abc");
    }
    for (size_t i = 0; i < 10; ++i) {
        strcat(a, "aaa");
        test(a);
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