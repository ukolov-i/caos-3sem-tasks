#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef enum {
    PlusZero = 0x00,
    MinusZero = 0x01,
    PlusInf = 0xF0,
    MinusInf = 0xF1,
    PlusRegular = 0x10,
    MinusRegular = 0x11,
    PlusDenormal = 0x20,
    MinusDenormal = 0x21,
    SignalingNaN = 0x30,
    QuietNaN = 0x31
} float_class_t;

const uint64_t exp_size = 11;
const uint64_t exp_bit = 52;
const uint64_t sign_bit = 63;
const uint64_t all_ones_exp = ~(~(uint64_t)0 << exp_size);

bool is_positive(uint64_t* value)
{
    return *value >> sign_bit == 0;
}

bool is_negative(uint64_t* value)
{
    return *value >> sign_bit == 1;
}

bool exp_has_bit_zero(uint64_t* value)
{
    return ((*value << 1) >> (exp_bit + 1)) != all_ones_exp;
}

bool exp_has_bit_one(uint64_t* value)
{
    return ((*value << 1) >> (exp_bit + 1) != 0);
}

bool frac_has_bit_one(uint64_t* value)
{
    return *value << (exp_size + 1) != 0;
}

bool leading_frac_bit(uint64_t* value)
{
    return ((*value << (exp_size + 1)) >> sign_bit) == 1;
}

extern float_class_t classify(double* value_ptr)
{
    float_class_t result = PlusRegular;
    uint64_t* data = (uint64_t*)value_ptr;
    if (!exp_has_bit_zero(data)) {
        if (frac_has_bit_one(data)) {
            if (leading_frac_bit(data)) {
                result = QuietNaN;
            } else {
                result = SignalingNaN;
            }
        } else {
            if (is_negative(data)) {
                result = MinusInf;
            } else {
                result = PlusInf;
            }
        }
    } else if (!exp_has_bit_one(data)) {
        if (frac_has_bit_one(data)) {
            if (is_negative(data)) {
                result = MinusDenormal;
            } else {
                result = PlusDenormal;
            }
        } else {
            if (is_negative(data)) {
                result = MinusZero;
            } else {
                result = PlusZero;
            }
        }
    } else if (is_negative(data)) {
        result = MinusRegular;
    }

    return result;
}