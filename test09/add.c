#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include "add.h"
#define ADD 32
// return the MIPS opcode for add $d, $s, $t
uint32_t add(uint32_t d, uint32_t s, uint32_t t) {
    s = s << 21;
    t = t << 16;
    d = d << 11;
    uint32_t return_d = s + t + d + ADD;
    return return_d; // REPLACE WITH YOUR CODE

}
