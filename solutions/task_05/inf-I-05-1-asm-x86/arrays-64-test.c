#include <stdlib.h>
#include <stdio.h>

#include "test_utility.h"


NOT_OPTIMIZE
void sum(int N, const int *A, const int *B, int *R);


NOT_OPTIMIZE void 
test(int N, const int *A, const int *B) {
    int R[1000];
    sum(N, A, B, R);

    int result = 1;
    for (int i = 0; i < N; ++i) {
        if (A[i] + B[i] != R[i]) {
            result = 0;
        }
    }
    LOG_RESULT(result, "   %d\n", N);
}

TEST(0) {
    #define SIZE 5
    START_TEST(0);

    int A[SIZE];
    int B[SIZE];
    for (int i = 0; i < SIZE; ++i) {
        A[i] = i;
        B[i] = 10 * i;
    }

    test(SIZE, A, B);

    #undef SIZE
}

NOT_OPTIMIZE void
run_tests() {
    LOG_STARTING_TESTS();

    test0();

    LOG_FINISHED_TESTS();
}


int main() {
    run_tests();
    return 0;
}