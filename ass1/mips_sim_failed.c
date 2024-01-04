// COMP1521 20T3 Assignment 1: mips_sim -- a MIPS simulator
// starting point code v0.1 - 13/10/20

//COMP1521 Assignment 1
//by z5311209 Celine Lin

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#include "mips_sim.h"

void execute_instructions(int n_instructions,
                          uint32_t instructions[n_instructions],
                          int trace_mode);
char *process_arguments(int argc, char *argv[], int *trace_mode);
uint32_t *read_instructions(char *filename, int *n_instructions_p);
uint32_t *instructions_realloc(uint32_t *instructions, int n_instructions);


// ADD YOUR FUNCTION PROTOTYPES HERE
struct assembler {
    struct assembler_char_int *assembler0;
    struct assembler_addr_int *assembler_S;
    struct assembler_addr_int *assembler_T;
    struct assembler_di *assembler_D_or_I;
    struct assembler *next;
};

struct assembler_char_int {
    char *char_assembler;
    uint32_t int_assembler;
};

struct assembler_addr_int {
    uint32_t addr_assembler;
    uint32_t int_assembler;
};

struct assembler_di {
    struct assembler_d *assembler_D;
    struct assembler_i *assembler_I;
};

struct assembler_d {
    uint32_t addr_assembler;
    uint32_t int_assembler;
};

struct assembler_i {
    uint32_t addr_assembler;
    uint32_t int_assembler;
};

Assembler instruction_in_struct_assemblr(int n_instructions, uint32_t instructions[n_instructions], int pc);
void check_beq_bne_addi_lui (uint32_t assembler_0,
                             uint32_t s_assembler, 
                             uint32_t t_assembler,
                             uint32_t i_assembler,
                             Assembler assembler);
void check_add_sub_slt_syscall_mul (uint32_t assembler_0,
                                    uint32_t s_assembler, 
                                    uint32_t t_assembler,
                                    uint32_t d_assembler,
                                    Assembler assembler);
void ori_function(uint32_t assembler_0,
                  uint32_t s_assembler, 
                  uint32_t t_assembler,
                  uint32_t i_assembler,
                  Assembler assembler);
void add_function(uint32_t assembler_0,
                  uint32_t s_assembler, 
                  uint32_t t_assembler,
                  uint32_t i_assembler,
                  Assembler assembler);

void syscall_function(uint32_t assembler_0,
                      uint32_t s_assembler, 
                      uint32_t t_assembler,
                      uint32_t i_assembler,
                      Assembler assembler);
void mul_function(uint32_t assembler_0,
                  uint32_t s_assembler, 
                  uint32_t t_assembler,
                  uint32_t i_assembler,
                  Assembler assembler);
// YOU SHOULD NOT NEED TO CHANGE MAIN

int main(int argc, char *argv[]) {
    int trace_mode;
    char *filename = process_arguments(argc, argv, &trace_mode);

    int n_instructions;
    uint32_t *instructions = read_instructions(filename, &n_instructions);

    execute_instructions(n_instructions, instructions, trace_mode);

    free(instructions);
    return 0;
}


// simulate execution of  instruction codes in  instructions array
// output from syscall instruction & any error messages are printed
//
// if trace_mode != 0:
//     information is printed about each instruction as it executed
//
// execution stops if it reaches the end of the array

void execute_instructions(int n_instructions,
                          uint32_t instructions[n_instructions],
                          int trace_mode) {

    struct assembler *assemblers = malloc(sizeof(struct assembler));

    char *assembler_0_char;
    uint32_t assembler_0_int;
    uint32_t s_addr_assembler;
    uint32_t t_addr_assembler;
    uint32_t d_addr_assembler;
    uint32_t i_addr_assembler;
    uint32_t s_int_assembler;
    uint32_t t_int_assembler;
    uint32_t d_int_assembler;
    uint32_t syscall_assembler = 0;
    uint32_t syscall = 1;
    int pc = 0;

    while (pc < n_instructions) {
        assemblers = instruction_in_struct_assemblr(n_instructions, instructions, pc);
        assembler_0_char = assemblers->assembler0->char_assembler;
        assembler_0_int = assemblers->assembler0->int_assembler;
        s_addr_assembler = assemblers->assembler_S->addr_assembler;
        t_addr_assembler = assemblers->assembler_T->addr_assembler;
        s_int_assembler = assemblers->assembler_S->int_assembler;
        t_int_assembler = assemblers->assembler_T->int_assembler;

        if ((pc+2) < n_instructions && instructions[pc+2] == SYSCALL) {
            syscall = assemblers->assembler_D_or_I->assembler_I->addr_assembler;
        }
        if ((pc+1) < n_instructions && instructions[pc+1] == SYSCALL) {
            syscall_assembler = assemblers->assembler_T->int_assembler;
        }
        if (trace_mode == 0 && pc == (n_instructions - 5)) {
            break;
        }
        if (trace_mode) {
            printf("%d: 0x%08X ", pc, instructions[pc]);
            if (instructions[pc] == SYSCALL) {//syscall
                printf("syscall\n");
                printf(">>> syscall %d\n", syscall_assembler);
                printf("<<< ");
                if (syscall_assembler != 11) {//print int
                    printf("%d", syscall);
                }
                printf("\n");
                
            } else if (assembler_0_int == 0 || assembler_0_int == MUL) {//$d $s $t
                d_addr_assembler = assemblers->assembler_D_or_I->assembler_D->addr_assembler;
                d_int_assembler = assemblers->assembler_D_or_I->assembler_D->int_assembler;
                printf("%s  $%d, $%d, $%d\n", 
                assembler_0_char, d_addr_assembler, s_addr_assembler, t_addr_assembler);
                printf(">>> $%d = %d\n", d_addr_assembler, d_int_assembler);
            } else {
                i_addr_assembler = assemblers->assembler_D_or_I->assembler_I->addr_assembler;
                
                if (assembler_0_int == BEQ || assembler_0_int == BNE) {//$s $t I
                    s_int_assembler = assemblers->assembler_S->int_assembler;
                    printf("%s  $%d, $%d, %d\n", 
                    assembler_0_char, s_addr_assembler, t_addr_assembler, i_addr_assembler);
                    printf(">>> $%d = %d\n", s_addr_assembler, s_int_assembler);

                } else if (assembler_0_int == LUI) {//lui $t I
                    printf("%s  $%d %d\n", 
                    assembler_0_char, t_addr_assembler, i_addr_assembler);
                    printf(">>> $%d = %d\n", t_addr_assembler, t_int_assembler);
                } else {//$t $s I
                    printf("%s  $%d, $%d, %d\n", 
                    assembler_0_char, t_addr_assembler, s_addr_assembler, i_addr_assembler);
                    printf(">>> $%d = %d\n", t_addr_assembler, t_int_assembler);
                }

            }
        }
        pc++;
    }
    if (trace_mode == 0) {
        printf("%d", syscall);
    }
    printf("\n");
}

Assembler instruction_in_struct_assemblr(int n_instructions, uint32_t instructions[n_instructions], int pc) {

    Assembler assembler = malloc(sizeof(struct assembler));

    Assembler_char_int assembler_char_int = malloc(sizeof(struct assembler_char_int));
    assembler->assembler0 = assembler_char_int;
    uint32_t assembler_0 = instructions[pc] >> 26;
    assembler->assembler0->int_assembler = assembler_0;

    Assembler_addr_int assembler_addr_int_s = malloc(sizeof(struct assembler_addr_int));
    assembler->assembler_S = assembler_addr_int_s;
    uint32_t s_assembler = instructions[pc] << 6;
    s_assembler = s_assembler >> 27;
    assembler->assembler_S->addr_assembler = s_assembler;

    Assembler_addr_int assembler_addr_int_t = malloc(sizeof(struct assembler_addr_int));
    assembler->assembler_T = assembler_addr_int_t;
    uint32_t t_assembler = instructions[pc] << 11;
    t_assembler = t_assembler >> 27;
    assembler->assembler_T->addr_assembler = t_assembler;

    Assembler_d_or_i assembler_DI = malloc(sizeof(struct assembler_di)) ;
    assembler->assembler_D_or_I = assembler_DI;
    uint32_t d_or_i_assembler = instructions[pc] << 16;
    if (assembler_0 == 0 || assembler_0 == MUL){//if add sub slt syscall mul (d)
        Assembler_d assembler_D = malloc(sizeof(struct assembler_d));
        assembler->assembler_D_or_I->assembler_D = assembler_D;
        uint32_t d_assembler = d_or_i_assembler >> 27;
        assembler->assembler_D_or_I->assembler_D->addr_assembler = d_assembler;

        assembler_0 = instructions[pc] << 26;
        assembler_0 = assembler_0 >> 26;
        check_add_sub_slt_syscall_mul(assembler_0, s_assembler, t_assembler, d_assembler, assembler);

    } else {// beq bne addi lui ori(I)
        Assembler_i assembler_I = malloc(sizeof(struct assembler_i));
        assembler->assembler_D_or_I->assembler_I = assembler_I;
        uint32_t i_assembler = d_or_i_assembler >> 16;
        assembler->assembler_D_or_I->assembler_I->addr_assembler = i_assembler;

        check_beq_bne_addi_lui(assembler_0, s_assembler, t_assembler, i_assembler, assembler);
    }

    return assembler;
}

void check_beq_bne_addi_lui (uint32_t assembler_0,
                             uint32_t s_assembler, 
                             uint32_t t_assembler,
                             uint32_t i_assembler,
                             Assembler assembler) {

    if (assembler_0 == ORI) {
       ori_function(assembler_0, s_assembler, t_assembler, i_assembler, assembler);
    } else if (assembler_0 == BNE) {
        //bne_function(assembler_0, s_assembler, t_assembler, i_assembler, assembler);
    } else if (assembler_0 == BEQ) {
        //beq_function(assembler_0, s_assembler, t_assembler, i_assembler, assembler);
    } else if (assembler_0 == ADDI) {
        //addi_function(assembler_0, s_assembler, t_assembler, i_assembler, assembler);
    } else if (assembler_0 ==LUI) {
       // lui_funtion()assembler_0, s_assembler, t_assembler, i_assembler, assembler;
    }
}

void check_add_sub_slt_syscall_mul (uint32_t assembler_0,
                                    uint32_t s_assembler, 
                                    uint32_t t_assembler,
                                    uint32_t d_assembler,
                                    Assembler assembler) {

    if (assembler_0 == ADD) {
        add_function(assembler_0, s_assembler, t_assembler, d_assembler, assembler);
    } else if (assembler_0 == SUB) {
        //sub_function(assembler_0, s_assembler, t_assembler, d_assembler, assembler);
    } else if (assembler_0 == SLT) {
        //slt_function(assembler_0, s_assembler, t_assembler, d_assembler, assembler);
    } else if (assembler_0 == SYSCALL) {
        syscall_function(assembler_0, s_assembler, t_assembler, d_assembler, assembler);
    } else {//assembler_0 == MUL
        mul_function(assembler_0, s_assembler, t_assembler, d_assembler, assembler);
    }
}
void ori_function(uint32_t assembler_0, uint32_t s_assembler, 
                  uint32_t t_assembler,uint32_t i_assembler,
                  Assembler assembler) {//t = s | I
    assembler->assembler0->char_assembler = "ori";
    assembler->assembler_T->int_assembler = s_assembler | i_assembler;
    
}
void syscall_function(uint32_t assembler_0, uint32_t s_assembler, 
                      uint32_t t_assembler, uint32_t i_assembler,
                      Assembler assembler) {
    assembler->assembler0->char_assembler = "syscall";
}


void add_function(uint32_t assembler_0, uint32_t s_assembler, 
                  uint32_t t_assembler, uint32_t i_assembler,
                  Assembler assembler) {//d = s + t
    assembler->assembler0->char_assembler = "add";
    uint32_t s = assembler->assembler_S->int_assembler;
    uint32_t t = assembler->assembler_T->int_assembler;
    uint32_t d = s + t;
    assembler->assembler_D_or_I->assembler_D->int_assembler = d;
}

void mul_function(uint32_t assembler_0, uint32_t s_assembler, 
                  uint32_t t_assembler, uint32_t i_assembler,
                  Assembler assembler) {//d = s * t
    assembler->assembler0->char_assembler = "mul";
    uint32_t s = assembler->assembler_S->int_assembler;
    uint32_t t = assembler->assembler_T->int_assembler;
    uint32_t d = s * t;
    printf("test:%d\n", s);
    assembler->assembler_D_or_I->assembler_D->int_assembler = d;
}


// YOU DO NOT NEED TO CHANGE CODE BELOW HERE


// check_arguments is given command-line arguments
// it sets *trace_mode to 0 if -r is specified
//          *trace_mode is set to 1 otherwise
// the filename specified in command-line arguments is returned

char *process_arguments(int argc, char *argv[], int *trace_mode) {
    if (
        argc < 2 ||
        argc > 3 ||
        (argc == 2 && strcmp(argv[1], "-r") == 0) ||
        (argc == 3 && strcmp(argv[1], "-r") != 0)) {
        fprintf(stderr, "Usage: %s [-r] <file>\n", argv[0]);
        exit(1);
    }
    *trace_mode = (argc == 2);
    return argv[argc - 1];
}


// read hexadecimal numbers from filename one per line
// numbers are return in a malloc'ed array
// *n_instructions is set to size of the array

uint32_t *read_instructions(char *filename, int *n_instructions_p) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        fprintf(stderr, "%s: '%s'\n", strerror(errno), filename);
        exit(1);
    }

    uint32_t *instructions = NULL;
    int n_instructions = 0;
    char line[MAX_LINE_LENGTH + 1];
    while (fgets(line, sizeof line, f) != NULL) {

        // grow instructions array in steps of INSTRUCTIONS_GROW elements
        if (n_instructions % INSTRUCTIONS_GROW == 0) {
            instructions = instructions_realloc(instructions, n_instructions + INSTRUCTIONS_GROW);
        }

        char *endptr;
        instructions[n_instructions] = strtol(line, &endptr, 16);
        if (*endptr != '\n' && *endptr != '\r' && *endptr != '\0') {
            fprintf(stderr, "%s:line %d: invalid hexadecimal number: %s",
                    filename, n_instructions + 1, line);
            exit(1);
        }
        n_instructions++;
    }
    fclose(f);
    *n_instructions_p = n_instructions;
    // shrink instructions array to correct size
    instructions = instructions_realloc(instructions, n_instructions);
    return instructions;
}


// instructions_realloc is wrapper for realloc
// it calls realloc to grow/shrink the instructions array
// to the speicfied size
// it exits if realloc fails
// otherwise it returns the new instructions array
uint32_t *instructions_realloc(uint32_t *instructions, int n_instructions) {
    instructions = realloc(instructions, n_instructions * sizeof *instructions);
    if (instructions == NULL) {
        fprintf(stderr, "out of memory");
        exit(1);
    }
    return instructions;
}