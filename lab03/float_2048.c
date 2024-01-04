// Multiply a float by 2048 using bit operations only

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#include "floats.h"

// float_2048 is given the bits of a float f as a uint32_t
// it uses bit operations and + to calculate f * 2048
// and returns the bits of this value as a uint32_t
//
// if the result is too large to be represented as a float +inf or -inf is returned
//
// if f is +0, -0, +inf or -inf, or Nan it is returned unchanged
//
// float_2048 assumes f is not a denormal number
//
uint32_t float_2048(uint32_t f) {
    uint32_t ff = f << 1;
    if (ff == 0x00000000 ||//0
        f == 0x7fc00000 ||//nan
        ff == 0xff000000 //inf
        ) {
        return f;
    } else {
        uint32_t elsb = 11;
        elsb = elsb << 23;
        uint32_t mul = elsb + f;
        
        uint32_t sign = f >> 31;
        uint32_t exp = f << 1;
        exp = exp >> 24;
        if (sign == 0x0 && exp >= 245){//pos inf
            uint32_t inf = 0x7f800000;
            return inf;
        } else if (sign == 0x1 && exp >= 245) {//nega inf
            uint32_t nega_inf = 0xff800000;
            return nega_inf;
        }
        return mul;
    }
}
