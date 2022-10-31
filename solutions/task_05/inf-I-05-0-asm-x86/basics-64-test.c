#include <stdlib.h>

#include "test_utility.h"

int* A;
void very_important_function(size_t N);


NOT_OPTIMIZE void 
test() {
    A = malloc(16);
    A[0] = 1;
    A[1] = 2;
    A[2] = 3;
    A[3] = 4;
    very_important_function(4);
    free(A);
}

TEST(0) {
    test();
}

NOT_OPTIMIZE void
run_tests() {
    LOG_STARTING_TESTS();

    test0();

    LOG_FINISHED_TESTS();
}


NOT_OPTIMIZE int 
main() {
    run_tests();
    return 0;
}