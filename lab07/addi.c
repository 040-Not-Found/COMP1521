// Sample solution for COMP1521 lab exercises
//
// generate the opcode for an addi instruction

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#include "addi.h"

#define MUL 8

// return the MIPS opcode for addi $t,$s, i
uint32_t addi(int t, int s, int i) {

    uint32_t mul = MUL;
    mul = mul << 26;

    uint32_t t_val = t << 16;
    uint32_t s_val = s << 21;
    uint16_t i_val = i;
    
    uint32_t return_instruction = mul|t_val|s_val|i_val;
    return return_instruction;
}
