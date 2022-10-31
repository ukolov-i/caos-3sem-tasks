#include <stdlib.h>
#include <stdio.h>

#include "test_utility.h"


void mergesort(int from, int to, const int* in, int* out);


NOT_OPTIMIZE void
test(int from, int to, const int* in, int* out) {
    mergesort(from, to, in, out);
    int result = 1;
	for (int i = 0; i < to - from - 1; ++i) {
		if (out[i] > out[i + 1]) {
            result = 0;
        }
    }
    LOG_RESULT(result, "   %d", to - from);
}

TEST(0) {
    START_TEST(0);

    int in[6] = { 12, 11, 13, 5, 6, 7 };
    int out[6] = { 0, 0, 0, 0, 0, 0};
	int size = sizeof(in) / sizeof(in[0]);

    test(0, size, in, out);
}

TEST(1) {
    START_TEST(1);

    int in[6] = { 12, 11, 13, 5, 6, 7 };
    int out[6] = { -1, -1, -1, -1, -1, -1};
	int size = sizeof(in) / sizeof(in[0]);

    test(1, 3, in, out);
	mergesort(1, 3, in, out);
    test(1, 3, in, out);
}

TEST(2) {
    START_TEST(2);

    int in[6] = { 12, 11, 13, 5, 6, 7 };
    int out[8] = { -2, -2, -2, -2, -2, -2, -2, -2};
	int size = sizeof(in) / sizeof(in[0]);
    
	test(1, 5, in, out + 3);
    test(2, 4, in, out + 2);
    test(0, 5, in, out + 1);
}

TEST(3) {
    START_TEST(3);

    int in[10] = { 12, 11, 13, 5, 6, 7, 127, 1000, 2, 9 };
    int out[14] = { -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2};
	int size = sizeof(in) / sizeof(in[0]);

	test(6, 9, in, out + 2);
	test(5, 8, in, out + 2);
	test(4, 7, in, out + 2);
	test(3, 6, in, out + 1);
	test(2, 5, in, out + 1);
	test(1, 4, in, out + 1);
}


TEST(4) {
    #define SIZE 123456
    START_TEST(4);
    
    int in[SIZE];
    int out[SIZE];
    for (int i = 0; i < SIZE; ++i) {
        in[i] = SIZE - i;
    }

    for (int i = 0; i < 8; ++i) {
	    test(10 * i, SIZE - 10 * i, in, out);
    }

    #undef SIZE
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
main() {
	run_tests();
	return 0;
}
