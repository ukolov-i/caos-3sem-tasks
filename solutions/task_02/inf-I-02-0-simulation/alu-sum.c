#include <stdint.h>

extern void sum(ITYPE first, ITYPE second, ITYPE* res, int* CF)
{
    ITYPE carry = 0;
    while (second) {
        ITYPE carry = first & second;
        first = first ^ second;
        second = carry << 1;
    }
    *res = first;
    *CF = (carry > 0);
}