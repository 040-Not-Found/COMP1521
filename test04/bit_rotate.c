#include "bit_rotate.h"

// return the value bits rotated left n_rotations
uint16_t bit_rotate(int n_rotations, uint16_t bits) {
    while (n_rotations < 0) {
        n_rotations += 16;
    }
    
    int mod_16 = n_rotations % 16;
    uint16_t rota = bits >> (16 - mod_16);
    bits = bits << mod_16;
    uint16_t out = bits | rota;
    return out;
}
