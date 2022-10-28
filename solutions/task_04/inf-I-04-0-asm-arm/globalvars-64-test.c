#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "test_utility.h"


uint64_t A = 1, B = 2, C = 3, D = 4;
extern uint64_t R;

void calculate();


NOT_OPTIMIZE void
test() {
    calculate();

    LOG_CONDITION(R == A * B + C * D, "PASSED", "FAILED", "   %lu %lu\n", R, A * B + C * D);
}

TEST(0) {
    START_TEST(0);

    test();
}

NOT_OPTIMIZE void
run_tests() {
    LOG(" --- STARTING TESTS --- ");

    test0();

    LOG(" --- FINISHED TESTS --- ");
}




NOT_OPTIMIZE int 
main(int argc, char **argv) {
    init_arch(argc, argv);
    run_tests();
    return 0;
}