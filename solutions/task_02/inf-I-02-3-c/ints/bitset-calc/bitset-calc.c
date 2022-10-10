

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int total_letters = 26;
const int total_digits = 10;

int get_bit_pos(char symbol)
{
    int bit_pos = 0;
    if (symbol <= '9') {
        bit_pos = symbol - '0';
    } else if (symbol <= 'Z') {
        bit_pos = symbol - 'A' + total_digits;
    } else {
        bit_pos = symbol - 'a' + total_digits + total_letters;
    }
    return bit_pos;
}

void make_intersection(uint64_t* left_set, uint64_t* right_set)
{
    *right_set &= *left_set;
    *left_set = 0;
}

void make_union(uint64_t* left_set, uint64_t* right_set)
{
    *right_set |= *left_set;
    *left_set = 0;
}

void make_xor(uint64_t* left_set, uint64_t* right_set)
{
    *right_set ^= *left_set;
    *left_set = 0;
}

void make_complement(uint64_t* left_set, uint64_t* right_set)
{
    *right_set = ~*right_set;
    *left_set = 0;
}

void insert_symbol(uint64_t* set, char element)
{
    *set |= (uint64_t)1 << get_bit_pos(element);
}

char get_data(int bit_pos)
{
    char symbol = 0;
    if (bit_pos < total_digits) {
        symbol = bit_pos + '0';
    } else if (bit_pos < total_digits + total_letters) {
        symbol = bit_pos + 'A' - total_digits;
    } else {
        symbol = bit_pos + 'a' - total_digits - total_letters;
    }
    return symbol;
}

int main()
{
    uint64_t left_set = 0;
    uint64_t right_set = 0;
    char operation = 0;

    while ((operation = getchar()) != -1) {
        if (operation == '&') {
            make_intersection(&left_set, &right_set);
        } else if (operation == '|') {
            make_union(&left_set, &right_set);
        } else if (operation == '^') {
            make_xor(&left_set, &right_set);
        } else if (operation == '~') {
            make_complement(&left_set, &right_set);
        } else {
            insert_symbol(&left_set, operation);
        }
    }

    for (int i = 0; i < (2 * total_letters) + total_digits; ++i) {
        if ((right_set >> i) & 1) {
            printf("%c", get_data(i));
        }
    }
}
