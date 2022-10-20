#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

bool is_invalid(int expected, uint8_t uint_data)
{
    return (expected && (uint_data >> 6) != 2) ||
           (!expected && (uint_data >> 6) == 2);
}

int main()
{
    int expected = 0;
    int accepted = 0;
    int rejected = 0;
    typedef union {
        char raw_data;
        uint8_t uint_data;
    } utf_block;
    utf_block data;

    while (scanf("%c", &data.raw_data) != EOF) {
        if (is_invalid(expected, data.uint_data)) {
            printf("%d %d\n", accepted, rejected);
            return 1;
        } else if (expected) {
            expected--;
            if (!expected) {
                rejected++;
            }
        } else if (!(data.uint_data >> 7)) {
            accepted++;
        } else if (data.uint_data < 0xDF) {
            expected = 1;
        } else if (data.uint_data < 0xEF) {
            expected = 2;
        } else {
            expected = 3;
        }
    }

    printf("%d %d\n", accepted, rejected);
    return 0;
}