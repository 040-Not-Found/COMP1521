// Online C compiler to run C program online
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

uint64_t bit_swap(uint64_t value) {
    uint64_t cur_value;
    uint64_t out = 0;
    for(int k = 0; k < 32; k++) {
        cur_value = value >> (2*k);
        cur_value = cur_value & 3;
        if(cur_value == 1) {
            cur_value = 2;
        } else if(cur_value == 2) {
            cur_value = 1;
        }
        cur_value = cur_value << (2*k);
        out += cur_value;
    }
    return out;
}
