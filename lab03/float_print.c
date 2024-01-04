// Print out a float.

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#include "floats.h"

//
// float_print is given the bits of a float as a uint32_t
// it prints out the float in the same format as "%.9g\n"
// using only putchar & puts
//
void float_print(uint32_t bits) {
    //sign
    uint32_t sig = bits >> 31;
    
    //exponent
    uint32_t exp = bits << 1;
    exp = exp >> 24;
    
    //fraction
    uint32_t frac = bits << 9;
    frac = frac >> 9;
}
