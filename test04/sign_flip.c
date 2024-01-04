#include "sign_flip.h"

// given the 32 bits of a float return it with its sign flipped
uint32_t sign_flip(uint32_t f) {
    uint32_t sign = 1;
    sign = sign << 31;
    
    uint32_t flip = sign ^ f;
    
    return flip;
}
