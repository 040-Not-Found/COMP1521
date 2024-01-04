//constants

#define MAX_LINE_LENGTH 256
#define INSTRUCTIONS_GROW 64
#define MAX_REGISTER 32
#define EXIT 10
#define ZERO 0
#define NEGATIVE 1
#define POSITIVE 0
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
