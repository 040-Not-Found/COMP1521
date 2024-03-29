#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

// given a uint32_t value,
// return 1 iff the least significant (bottom) byte
// is equal to the 2nd least significant byte; and
// return 0 otherwise
int practice_q2(uint32_t value) {
    uint32_t least = value << 16;
    least = least >> 24;

    uint32_t sec_least = value << 24;
    sec_least = sec_least >> 24;

    if (least == sec_least) {
        return 1;
    }

    return 0;
}
