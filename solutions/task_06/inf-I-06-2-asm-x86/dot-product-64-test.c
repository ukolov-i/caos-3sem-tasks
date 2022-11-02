#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <immintrin.h>
#include <math.h>

#include "test_utility.h"


NOT_OPTIMIZE float 
dot_product(size_t N, const float *A, const float *B);

NOT_OPTIMIZE float 
naive_dot_product(size_t N, const float *A, const float *B) {
    float result = 0;
    for (int i = 0; i < N; ++i) {
        result += A[i] * B[i];
    }
    return result;
}


NOT_OPTIMIZE void 
test(size_t N, const float *A, const float *B) {
    float r1 = dot_product(N, A, B);
    float r2 = naive_dot_product(N, A, B);
    LOG_RESULT(EQUAL_FLOAT(r1, r2), "   %ld\n   %lf %lf\n", N, r1, r2)
}

TEST(0) {
    START_TEST(0);
    #define TEST_SIZE 2

    float A[TEST_SIZE] = {0, 0};
    float B[TEST_SIZE] = {0, 0};
    test(TEST_SIZE, A, B);
}

TEST(1) {
    #define TEST_SIZE 3
    START_TEST(1);
    
    float A[TEST_SIZE] = {1, 2, 3};
    test(TEST_SIZE, A, A);

    float B[TEST_SIZE] = {4, 5, 6};
    test(TEST_SIZE, B, B);

    #undef TEST_SIZE
}

TEST(2) {
    #define TEST_SIZE 6
    START_TEST(2);
    
    float A[TEST_SIZE] = {1, 2, 3, 4, 5, 6};
    float B[TEST_SIZE] = {4, 5, 6, 1, 2, 3};

    test(TEST_SIZE, A, B);
    test(TEST_SIZE, B, B);

    #undef TEST_SIZE
}

TEST(3) {
    #define TEST_SIZE 6
    START_TEST(3);

    float A[TEST_SIZE] = {1, 2, 3, 4, 5, 6};
    float B[TEST_SIZE] = {4, 5, 6, 1, 2, 3};

    test(TEST_SIZE, A, B);
    test(TEST_SIZE, B, B);

    #undef TEST_SIZE
}

TEST(4) {
    #define TEST_SIZE 1009
    START_TEST(4);
    
    float A[TEST_SIZE];
    float B[TEST_SIZE];

    for (int i = 0; i < TEST_SIZE; ++i) {
        A[i] = 2 + i;
        B[i] = 2 - i;
    }

    test(TEST_SIZE, A, A);
    test(TEST_SIZE, A, B);
    test(TEST_SIZE, B, B);

    #undef TEST_SIZE
}

TEST(5) {
    #define TEST_SIZE 26
    START_TEST(5);
    
    float A[TEST_SIZE];
    float B[TEST_SIZE];

    for (int i = 0; i < TEST_SIZE; ++i) {
        A[i] = 0.1;
        B[i] = 0.1 - 0.2 * (i & 1);
    }
    
    test(TEST_SIZE, A, A);
    test(TEST_SIZE, A, B);
    test(TEST_SIZE, B, B);

    #undef TEST_SIZE
}

TEST(6) {
    #define TEST_SIZE 26
    START_TEST(6);
    
    float A[TEST_SIZE];
    float B[TEST_SIZE];

    for (int i = 0; i < TEST_SIZE; ++i) {
        A[i] = 0.0123 + i * 0.0123;
        B[i] = 0.1 - i * 0.01;
    }
    
    test(TEST_SIZE, A, A);
    test(TEST_SIZE, A, B);
    test(TEST_SIZE, B, B);

    #undef TEST_SIZE
}

TEST(7) {
    #define TEST_SIZE 123456
    #define TEST_AMOUNT 12345
    START_TEST(7);
    
    float* A = malloc(TEST_SIZE * sizeof(float));
    if (A == NULL) {
        LOG_FAILURE("ALLOCATION FAILED");
        return;
    }
    float* B = malloc(TEST_SIZE * sizeof(float));
    if (B == NULL) {
        free(A);
        LOG_FAILURE("ALLOCATION FAILED");
        return;
    }

    for (int i = 0; i < TEST_SIZE; ++i) {
        A[i] = 0.0123 + i * 0.0123;
        B[i] = 0.1 - i * 0.01;
    }
    
    for (int i = 0; i < 6; ++i) {
        test(TEST_AMOUNT, A + i * 10, A);
        test(TEST_AMOUNT, A + i * 100, B + i * 100);
        test(TEST_AMOUNT, B + 123 * i, B + 1234 * i);
    }

    free(A);
    free(B);
    #undef TEST_AMOUNT
    #undef TEST_SIZE
}

TEST(8) {
    #define TEST_SIZE 234555
    #define TEST_AMOUNT 12345
    #define TEST_ALIGNMENT 15
    STATIC_ASSERT(TEST_SIZE % TEST_ALIGNMENT == 0, "Size must be multiple of alignment");
    START_TEST(8);
    
    float* A = aligned_alloc(TEST_ALIGNMENT, TEST_SIZE * sizeof(float));
    if (A == NULL) {
        LOG_FAILURE("ALLOCATION FAILED");
        return;
    }
    float* B = aligned_alloc(TEST_ALIGNMENT, TEST_SIZE * sizeof(float));
    if (B == NULL) {
        free(A);
        LOG_FAILURE("ALLOCATION FAILED");
        return;
    }

    for (int i = 0; i < TEST_SIZE; ++i) {
        A[i] = 0.0123 + i * 0.0123;
        B[i] = 0.1 - i * 0.01;
    }
    
    for (int i = 0; i < 6; ++i) {
        test(TEST_AMOUNT, A + i * 10, A);
        test(TEST_AMOUNT, A + i * 100, B + i * 100);
        test(TEST_AMOUNT, B + 123 * i, B + 1234 * i);
    }

    free(A);
    free(B);
    #undef TEST_AMOUNT
    #undef TEST_SIZE
}

void run_tests() {
    LOG_STARTING_TESTS();

    test0();
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();
    test7();
    test8();

    LOG_FINISHED_TESTS();
}


NOT_OPTIMIZE void 
benchmark(size_t N, const float *A, const float *B) {
    clock_t tic = clock();
    float r1 = dot_product(N, A, B);
    clock_t toc = clock();
    double elapsed_first = (double)(toc - tic) / CLOCKS_PER_SEC;
    
    tic = clock();
    float r2 = naive_dot_product(N, A, B);
    toc = clock();
    double elapsed_second = (double)(toc - tic) / CLOCKS_PER_SEC;

    LOG_RESULT(elapsed_first < elapsed_second, "   %ld\n", N)
    printf("   %f %f\n", elapsed_first, elapsed_second);
}

BENCHMARK(0) {
    #define TEST_SIZE (int)(1e5)
    START_BENCHMARK(0)

    float A[TEST_SIZE];
    float B[TEST_SIZE];

    for (int i = 0; i < TEST_SIZE; ++i) {
        A[i] = 0.0123 + i * 0.0123;
        B[i] = 0.1 - i * 0.01;
    }

    benchmark(TEST_SIZE, A, B);

    #undef TEST_SIZE
}

BENCHMARK(1) {
    #define TEST_SIZE (int)(1e8)
    START_BENCHMARK(1)

    float* A = malloc(TEST_SIZE * sizeof(float));
    if (A == NULL) {
        LOG_FAILURE("ALLOCATION FAILED");
        return;
    }
    float* B = malloc(TEST_SIZE * sizeof(float));
    if (B == NULL) {
        free(A);
        LOG_FAILURE("ALLOCATION FAILED");
        return;
    }

    for (int i = 0; i < TEST_SIZE; ++i) {
        A[i] = 0.0123 + i * 0.0123;
        B[i] = 0.1 - i * 0.01;
    }

    benchmark(TEST_SIZE, A, B);

    free(A);
    free(B);
    #undef TEST_SIZE
}

BENCHMARK(2) {
    #define TEST_SIZE (int)(1e8)
    #define TEST_AMOUNT (int)(9 * 1e7)
    #define TEST_ALIGNMENT 10
    STATIC_ASSERT(TEST_SIZE % TEST_ALIGNMENT == 0, "Size must be multiple of alignment");
    START_BENCHMARK(2)

    float* A = aligned_alloc(TEST_ALIGNMENT, TEST_SIZE * sizeof(float));
    if (A == NULL) {
        LOG_FAILURE("ALLOCATION FAILED");
        return;
    }
    float* B = aligned_alloc(TEST_ALIGNMENT, TEST_SIZE * sizeof(float));
    if (B == NULL) {
        free(A);
        LOG_FAILURE("ALLOCATION FAILED");
        return;
    }

    for (int i = 0; i < TEST_SIZE; ++i) {
        A[i] = 0.0123 + i * 0.0123;
        B[i] = 0.1 - i * 0.01;
    }

    benchmark(TEST_AMOUNT, A, B);

    free(A);
    free(B);
    #undef TEST_AMOUNT
    #undef TEST_SIZE
}

BENCHMARK(3) {
    #define TEST_SIZE (int)(1e9)
    START_BENCHMARK(3)

    float* A = malloc(TEST_SIZE * sizeof(float));
    if (A == NULL) {
        LOG_FAILURE("ALLOCATION FAILED");
        return;
    }
    float* B = malloc(TEST_SIZE * sizeof(float));
    if (B == NULL) {
        free(A);
        LOG_FAILURE("ALLOCATION FAILED");
        return;
    }

    for (int i = 0; i < TEST_SIZE; ++i) {
        A[i] = 0.0123 + i * 0.0123;
        B[i] = 0.1 - i * 0.01;
    }

    benchmark(TEST_SIZE, A, B);

    free(A);
    free(B);
    #undef TEST_SIZE
}

BENCHMARK(4) {
    #define TEST_SIZE (int)(1e9)
    #define TEST_AMOUNT (int)(9 * 1e8)
    #define TEST_ALIGNMENT 10
    STATIC_ASSERT(TEST_SIZE % TEST_ALIGNMENT == 0, "Size must be multiple of alignment");
    START_BENCHMARK(4)

    float* A = aligned_alloc(TEST_ALIGNMENT, TEST_SIZE * sizeof(float));
    if (A == NULL) {
        LOG_FAILURE("ALLOCATION FAILED");
        return;
    }
    float* B = aligned_alloc(TEST_ALIGNMENT, TEST_SIZE * sizeof(float));
    if (B == NULL) {
        free(A);
        LOG_FAILURE("ALLOCATION FAILED");
        return;
    }

    for (int i = 0; i < TEST_SIZE; ++i) {
        A[i] = 0.0123 + i * 0.0123;
        B[i] = 0.1 - i * 0.01;
    }

    benchmark(TEST_AMOUNT, A, B);

    free(A);
    free(B);
    #undef TEST_AMOUNT
    #undef TEST_SIZE
}

NOT_OPTIMIZE void 
run_benchmarks() {
    LOG_STARTING_BENCMARKS();

    benchmark0();
    benchmark1();
    benchmark2();
    benchmark3();
    benchmark4();

    LOG_FINISHED_BENCMARKS();
}


NOT_OPTIMIZE int 
main() {
    run_tests();
    // run_benchmarks();
    return 0;
}   