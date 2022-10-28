#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "test_utility.h"


NOT_OPTIMIZE char*
char_filter_digits(char* str) {
    int curr = 0;
    int len = strlen(str);
    for (int i = 0; i < len; ++i) {
        char c = str[i];
        if ('0' <= c && c <= '9') {
            str[curr] = c;
            ++curr;
        }
    }
    for (int i = curr; i < len; ++i) {
        str[curr] = 0;
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

    int status = strcmp(char_filter_digits(copy), buffer);
    LOG_CONDITION(status == 0, "PASSED", "FAILED", "   %s %s\n", copy, buffer);
    free(copy);
}

TEST(0) {
    START_TEST(0);

    char a[] = "a1";
    test(a);
}

TEST(1) {
    START_TEST(1);

    char a[] = "abc";
    test(a);
    char b[] = "5678Hello world!1234";
    test(b);
}

TEST(2) {
    START_TEST(2);

    char a[100];
    strcpy(a, "a");
    for (size_t i = 0; i < 40; ++i) {
        strcat(a, "1c");
    }
    test(a);
    strcat(a, "c25a");
    test(a);
}

TEST(3) {
    START_TEST(3);

    char a[100];
    strcpy(a, "\n");
    test(a);
}

TEST(4) {
    START_TEST(4);

    char a[600];
    strcpy(a, "Hello world!");
    for (size_t i = 0; i < 140; ++i) {
        strcat(a, "21c");
    }
    for (size_t i = 0; i < 10; ++i) {
        strcat(a, "a96");
        test(a);
    }
}

NOT_OPTIMIZE void
run_tests() {
    LOG(" --- STARTING TESTS --- ");

    test0();
    test1();
    test2();
    test3();
    test4();

    LOG(" --- FINISHED TESTS --- ");
}


NOT_OPTIMIZE int
main(int argc, char **argv) {
    init_arch(argc, argv);
    run_tests();
    quit();
    return 0;
}