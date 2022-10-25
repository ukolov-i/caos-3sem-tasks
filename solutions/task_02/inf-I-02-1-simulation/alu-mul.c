#include <stdint.h>

void sum(ITYPE first, ITYPE second, ITYPE* res, int* CF)
{
    ITYPE carry = 0;
    while (second) {
        ITYPE carry = first & second;
        first = first ^ second;
        second = carry << 1;
    }
    *res = first;
    *CF = *CF | (carry > 0);
}

extern void mul(ITYPE first, ITYPE second, ITYPE* res, int* CF)
{
    *res = 0;
    *CF = 0;
    while (second) {
        if (second & 1) {
            sum(first, *res, res, CF);
        }
        if ((first ^ ((first << 1) >> 1)) && (second > 1)) {
            *CF = 1;
        }
        first = first << 1;
        second = second >> 1;
    }
}