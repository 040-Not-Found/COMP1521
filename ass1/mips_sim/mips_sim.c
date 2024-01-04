// COMP1521 20T3 Assignment 1: mips_sim -- a MIPS simulator
// starting point code v0.1 - 13/10/20

//comp1521 assignment1
//by z5311209 Celine Lin


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

//CONSTANTS
#define MAX_LINE_LENGTH 256
#define INSTRUCTIONS_GROW 64
#define MAX_REGISTER 32
#define EXIT 10
#define ZERO 0
#define BAD_SYSCALL 127
#define MAX_INSTRUCTION_LINES 20000

#define ADD 32 //tail
#define SUB 34 //tail
#define SLT 42 //tail
#define MUL 28 //head
#define SYSCALL 12

#define BEQ 4 //head
#define BNE 5 //head
#define ADDI 8 //head
#define ORI 13 //head
#define LUI 15 //head

//FUNCTIONS
void execute_instructions(int n_instructions,
                          uint32_t instructions[n_instructions],
                          int trace_mode);
char *process_arguments(int argc, char *argv[], int *trace_mode);
uint32_t *read_instructions(char *filename, int *n_instructions_p);
uint32_t *instructions_realloc(uint32_t *instructions, int n_instructions);

char *find_assembler_char(uint32_t assembler_head6, uint32_t assembler_tail11, 
                          uint32_t s_assembler, uint32_t t_assembler, 
                          uint32_t d_assembler, uint32_t i_assembler, 
                          int register_array[MAX_REGISTER]);
void ori_function(uint32_t s_assembler, uint32_t t_assembler, 
                  uint32_t i_assembler, int register_array[MAX_REGISTER]);
void add_function(uint32_t s_assembler, uint32_t t_assembler,
                  uint32_t d_assembler, int register_array[MAX_REGISTER]);
void sub_function(uint32_t s_assembler, uint32_t t_assembler,
                  uint32_t d_assembler, int register_array[MAX_REGISTER]);
void slt_function(uint32_t s_assembler, uint32_t t_assembler,
                  uint32_t d_assembler, int register_array[MAX_REGISTER]);
void mul_function(uint32_t s_assembler, uint32_t t_assembler,
                  uint32_t d_assembler, int register_array[MAX_REGISTER]);
void lui_function(uint32_t t_assembler, uint32_t i_assembler, 
                  int register_array[MAX_REGISTER]);
void addi_function(uint32_t s_assembler, uint32_t t_assembler, 
                   uint32_t i_assembler, int register_array[MAX_REGISTER]);
int beq_function(uint32_t s_assembler, uint32_t t_assembler, 
                 uint32_t i_assembler, int register_array[MAX_REGISTER]);
int bne_function(uint32_t s_assembler, uint32_t t_assembler, 
                 uint32_t i_assembler, int register_array[MAX_REGISTER]);
int check_errors(int n_instructions,
                 uint32_t instructions[n_instructions],
                 char *assembler_0_char, int syscall_assembler,
                 int pc, int *no_output, int return_pc, 
                 int jump, int trace_mode);
int find_syscall(int n_instructions,
                 uint32_t instructions[n_instructions],
                 uint32_t assembler_1, int register_array[MAX_REGISTER],
                 int pc, int syscall, char * assembler_0_char);
void find_output(int n_instructions, uint32_t instructions[n_instructions],
                 int pc, int syscall, int output[MAX_INSTRUCTION_LINES], 
                 int *counter);
int find_syscall_assembler(uint32_t assembler_1, uint32_t assembler_3, 
                           int syscall_assembler);
void print_syscall(int syscall_assembler, int syscall);



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

    uint32_t assembler_head6;
    uint32_t assembler_tail11;
    uint32_t s_assembler;
    uint32_t t_assembler;
    uint32_t d_assembler;
    int i_assembler;

    int assembler_1 = 0;
    int assembler_2 = 0;
    int assembler_3 = 0;

    int output[MAX_INSTRUCTION_LINES] = {};
    int no_output = 0;
    int counter = 0;
    int i = 0;
    int return_pc = MAX_LINE_LENGTH;
    int jump = 1;
    int syscall_assembler = 11;
    int syscall = {0};

    int register_array[MAX_REGISTER] = {};

    int pc = 0;
    while (pc < n_instructions) {

        //$0 will always be zero
        register_array[0] = 0;
        //pc++ if the assembler != bne or beq
        jump = 1;

        //seperate the instructions into
        //different parts
        assembler_head6 = instructions[pc] >> 26;

        assembler_tail11 = instructions[pc] << 21;
        assembler_tail11 = assembler_tail11 >> 21;

        s_assembler = instructions[pc] << 6;
        s_assembler = s_assembler >> 27;

        t_assembler = instructions[pc] << 11;
        t_assembler = t_assembler >> 27;

        d_assembler = instructions[pc] << 16;
        d_assembler = d_assembler >> 27;

        i_assembler = instructions[pc] << 16;
        i_assembler = i_assembler >> 16;
        
        //find the name(in char) of the instruction
        //and this will go to the void function of 
        //the instruction and do the work in the void 
        //function
        char *assembler_0_char = find_assembler_char(assembler_head6, assembler_tail11,
                                                    s_assembler, t_assembler,d_assembler,
                                                    i_assembler, register_array);
        
        //for printing
        //put the assembler in order
        //it's easier for printing
        if (assembler_head6 == ADDI || assembler_head6 == ORI) {//$t $s I
            assembler_1 = t_assembler;
            assembler_2 = s_assembler;
            assembler_3 = i_assembler;
        } else if (assembler_head6 == 0 || assembler_head6 == MUL) {//$d $s $t
            assembler_1 = d_assembler;
            assembler_2 = s_assembler;
            assembler_3 = t_assembler;
        } else if (assembler_head6 == LUI) {//$t I
            assembler_1 = t_assembler;
            assembler_2 = i_assembler;
        } else if (assembler_head6 == BNE || assembler_head6 == BEQ) {//$s $t I
            assembler_1 = s_assembler;
            assembler_2 = t_assembler;
            assembler_3 = i_assembler;
            //found the pc where the function is going to
            if (assembler_head6 == BEQ) {//if (s == t) pc += I
                return_pc = pc + beq_function(s_assembler, t_assembler, 
                                              i_assembler, register_array);
            } else {//if (s != t) pc += I
                return_pc = pc + bne_function(s_assembler, t_assembler, 
                                              i_assembler, register_array);
            }
            //if the current pc = next pc
            //the function will going an
            //endless loop
            //so jump to the next pc
            //when current pc != next pc
            if (pc != return_pc) {
                jump = 0;
            } else {//this will print "branch not taken"
                jump = 2;
            }
        }

        if (trace_mode) {//print function
            //print the instruction in hex
            printf("%d: 0x%08X ", pc, instructions[pc]);

            if (instructions[pc] == SYSCALL) {//print syscall
                print_syscall(syscall_assembler, syscall);
            } else if (assembler_head6 == LUI) {//print struct: lui  $t, I
                printf("%s  $%d, %d\n", 
                assembler_0_char, assembler_1, assembler_2);
                printf(">>> $%d = %d\n", 
                assembler_1, register_array[assembler_1]);
            } else if (instructions[pc] != 0 && assembler_0_char != NULL) {
                //print struct: $d $s $t
                if (assembler_head6 == 0 || assembler_head6 == MUL) {
                    printf("%s  $%d, $%d, $%d\n", 
                    assembler_0_char, assembler_1, assembler_2, assembler_3);
                } else if (assembler_head6 == ADDI) {//print struct: addi $t, $s, I
                    printf("%s $%d, $%d, %d\n", 
                    assembler_0_char, assembler_1, assembler_2, assembler_3);
                } else {//I
                    printf("%s  $%d, $%d, %d\n", 
                    assembler_0_char, assembler_1, assembler_2, assembler_3);
                }
                //print the value in assembler_1
                if (assembler_head6 != BEQ && assembler_head6 != BNE) {
                    printf(">>> $%d = %d\n", 
                    assembler_1, register_array[assembler_1]);
                } else if (pc != return_pc){//print success looping
                    printf(">>> branch taken to PC = %d\n", return_pc);
                } else {//print not 
                    printf(">>> branch not taken\n");
                }
            }
        }

        //check errors, if there's an error, break the loop
        int break_loop = check_errors(n_instructions, instructions,
                                     assembler_0_char, syscall_assembler, pc,
                                     &no_output, return_pc, jump, trace_mode);
        if (break_loop == 1) {
            break;
        }

        //find the syscall
        syscall = find_syscall(n_instructions, instructions,
                                    assembler_1, register_array, pc,
                                    syscall, assembler_0_char);
        //add syscall to output
        find_output(n_instructions, instructions, pc, 
                    syscall, output, &counter);
        //find the syscall instruction i.e. 1 is print integer
        syscall_assembler = find_syscall_assembler(assembler_1, assembler_3, 
                                                   syscall_assembler);

        if (jump == 0) {//jump to another branch
            pc = return_pc;
        } else {//jump = 1, keep going
            pc++;
        }
        
        //for trace mood (-r)
        //for some errors, there's no output
        if (trace_mode == 0 && no_output == 0 && i < counter) {
            if (syscall_assembler == 1) {//print integer
                printf("%d", output[i]);
            } else {//syscall_assembler == 11, print character
                printf("%c", output[i]);
            }
            i++;
        }
    }
    
}


//check errors and exit, such as
//zero
//bad instruction
//bad syscall
//bad branch
//if there's an error, the function
//will return 1, and this will break
//the loop in void execute_instructions
int check_errors(int n_instructions,
                 uint32_t instructions[n_instructions],
                 char *assembler_0_char, int syscall_assembler,
                 int pc, int *no_output, int return_pc, 
                 int jump, int trace_mode) {
    int break_loop = 0;
    if (instructions[pc] == ZERO) {//zero
        break_loop = 1;
        *no_output = 1;
    }
    if (assembler_0_char == NULL) {//bad instrction
        break_loop = 1;
        printf("invalid instruction code\n");
    }
    if (return_pc < 0) {//bad branch
        break_loop = 1;
        printf("Illegal branch to address before instructions: PC = %d\n", return_pc);
    } 
    if (syscall_assembler != 1 && syscall_assembler != 11) {
        break_loop = 1;
        if (syscall_assembler == EXIT) {//exit
            //exit without print anything
        } else {//bad syscall
            *no_output = 1;
            printf("Unknown system call: %d\n", syscall_assembler);
        }
    }

    return break_loop;
}

//the syscall will be the value in the register
//as bne, beq and slt functions does not put
//values in register, so the function will
//just return the previous syscall value
//and if ori $2 $0 $x, this mean tells the 
//type of the syscall, so the value in the 
//register $2 is not a syscall value
int find_syscall(int n_instructions,
                 uint32_t instructions[n_instructions],
                 uint32_t assembler_1, int register_array[MAX_REGISTER],
                 int pc, int syscall, char * assembler_0_char) {
    int return_syscall = syscall;
    if (strcmp(assembler_0_char, "bne") != 0 && strcmp(assembler_0_char, "beq") != 0) {
        if (strcmp(assembler_0_char, "slt") != 0) {
            if (assembler_1 != 2 && instructions[pc] != SYSCALL) {
                return_syscall = register_array[assembler_1];
            }
        }
    }
    return return_syscall;
}

//if ori $2 $0 $x, this mean tells the 
//type of the syscall, which is the
//syscall_assembler
int find_syscall_assembler(uint32_t assembler_1, uint32_t assembler_3, 
                           int syscall_assembler) {
    int return_syscall_assembler = syscall_assembler;
    if (assembler_1 == 2) {
        return_syscall_assembler = assembler_3;
    }
    return return_syscall_assembler;
}

//print the syscall value
//if syscall_assembler is 1, this
//will print the syscall in integer
//if syscall_assembler = 11, print char
void print_syscall(int syscall_assembler, int syscall) {
    printf("syscall\n");
    printf(">>> syscall %d\n", syscall_assembler);
    if (syscall_assembler == 1) {
        printf("<<< %d\n", syscall);
    } else if (syscall_assembler == 11) {
        char char_syscall = syscall;
        printf("<<< %c\n", char_syscall);
    } 
    
}

//for trace mood
//add the syscall value to the
//array, output
void find_output(int n_instructions, uint32_t instructions[n_instructions],
                int pc, int syscall, int output[MAX_INSTRUCTION_LINES], int *counter) {
    if (instructions[pc] == SYSCALL) {
        output[*counter] = syscall;
        *counter = *counter + 1;
    }
}

//find the type of assembler
//go to its void function to do the works
//and return its type name
char *find_assembler_char(uint32_t assembler_head6, uint32_t assembler_tail11, 
                          uint32_t s_assembler, uint32_t t_assembler, 
                          uint32_t d_assembler, uint32_t i_assembler, 
                          int register_array[MAX_REGISTER]) {
    char *find_assembler_char = NULL;
    if (assembler_head6 == 0) {
        if (assembler_tail11 == ADD) {
            find_assembler_char = "add";
            add_function(s_assembler, t_assembler, d_assembler, register_array);
        } else if (assembler_tail11 == SUB) {
            find_assembler_char = "sub";
            sub_function(s_assembler, t_assembler, d_assembler, register_array);
        } else if (assembler_tail11 == SLT) {
            find_assembler_char = "slt";
            slt_function(s_assembler, t_assembler, d_assembler, register_array);
        } else if (assembler_tail11 == SYSCALL) {
            find_assembler_char = "syscall";
        }
    } else if (assembler_head6 == MUL ) {
        find_assembler_char = "mul";
        mul_function(s_assembler, t_assembler, d_assembler, register_array);
    } else if (assembler_head6 == ORI) {
        find_assembler_char = "ori";
        ori_function(s_assembler, t_assembler, i_assembler, register_array);
    } else if (assembler_head6 == BNE) {
        find_assembler_char = "bne";
    } else if (assembler_head6 == BEQ) {
        find_assembler_char = "beq";
    } else if (assembler_head6 == ADDI) {
        find_assembler_char = "addi";
        addi_function(s_assembler, t_assembler, i_assembler, register_array);
    } else if (assembler_head6 ==LUI) {
        find_assembler_char = "lui";
        lui_function(t_assembler, i_assembler, register_array);
    }
    return find_assembler_char;
}

void ori_function(uint32_t s_assembler, uint32_t t_assembler, 
                 uint32_t i_assembler, int register_array[MAX_REGISTER]) {
    int s = register_array[s_assembler];
    int i = i_assembler;
    int t = s | i;
    register_array[t_assembler] = t;
}

void add_function(uint32_t s_assembler, uint32_t t_assembler,
                  uint32_t d_assembler, int register_array[MAX_REGISTER]) {
    int s = register_array[s_assembler];
    int t = register_array[t_assembler];
    int d = s + t;
    register_array[d_assembler] = d;
}

void sub_function(uint32_t s_assembler, uint32_t t_assembler,
                  uint32_t d_assembler, int register_array[MAX_REGISTER]) {
    int s = register_array[s_assembler];
    int t = register_array[t_assembler];
    int d = s - t;
    register_array[d_assembler] = d;
}

void slt_function(uint32_t s_assembler, uint32_t t_assembler,
                  uint32_t d_assembler, int register_array[MAX_REGISTER]) {
    int s = register_array[s_assembler];
    int t = register_array[t_assembler];
    int d = s < t;
    register_array[d_assembler] = d;
}

void mul_function(uint32_t s_assembler, uint32_t t_assembler,
                  uint32_t d_assembler, int register_array[MAX_REGISTER]) {
    int s = register_array[s_assembler];
    int t = register_array[t_assembler];
    int d = s * t;
    register_array[d_assembler] = d;
}

void addi_function(uint32_t s_assembler, uint32_t t_assembler, 
                   uint32_t i_assembler, int register_array[MAX_REGISTER]) {
    uint32_t i = i_assembler;
    int s = register_array[s_assembler];
    uint32_t t = s + i;
    register_array[t_assembler] = t;
}

void lui_function(uint32_t t_assembler, uint32_t i_assembler, 
                  int register_array[MAX_REGISTER]) {
    int i = i_assembler;
    int t = i << 16;
    register_array[t_assembler] = t;
}

int beq_function(uint32_t s_assembler, uint32_t t_assembler, 
                  uint32_t i_assembler, int register_array[MAX_REGISTER]) {
    uint32_t s = register_array[s_assembler];
    uint32_t t = register_array[t_assembler];
    if (s == t) {
        return i_assembler;
    }
    return 0;
}

int bne_function(uint32_t s_assembler, uint32_t t_assembler, 
                  uint32_t i_assembler, int register_array[MAX_REGISTER]) {
    uint32_t s = register_array[s_assembler];
    uint32_t t = register_array[t_assembler];
    if (s != t) {
        return i_assembler;
    }
    return 0;
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