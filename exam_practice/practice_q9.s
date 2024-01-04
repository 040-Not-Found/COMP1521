# this code reads a line of input and prints 42
# change it to evaluate the arithmetic expression

main:
    la   $a0, line
    la   $a1, 10000
    li   $v0, 8          # fgets(buffer, 256, stdin)
    syscall              #

    addi $sp $sp -4
    sw   $ra 0($sp)

    jal  expression

    lw   $ra, 0($sp)
    addi $sp $sp 4

    move $a0, $v0         # printf("%d");
    li   $v0, 1
    syscall

    li   $a0, '\n'       # printf("%c", '\n');
    li   $v0, 11
    syscall

    li   $v0, 0          # return 0
    jr   $31

expression:
    addi $sp $sp -8
    sw   $ra 0($sp)
    jal  number
    sw   $v0 4($sp)     #left

    la   $t1 line       #*s
    lw   $t2 ($t1)

    bne  $t2 '+' end_exp_left
        
    addi $t1 $t1 1

    addi $sp $sp -4
    sw   $ra 0($sp)
    jal  expression
    move $t0 $v0        #right

    lw   $ra 0($sp)
    lw   $t1 4($sp) 
    addi $sp $sp 12

    add  $t3 $t1 $t0
    move $v0 $t3

    jr   $ra

end_exp_left:
    lw   $ra, 0($sp)
    addi $sp $sp 8

    move $v0 $s0
    jr   $ra


term:
    addi $sp $sp -8
    sw   $ra 0($sp)
    jal  number
    sw   $v0 4($sp)     #left

    la   $t1 line       #*s
    lw   $t2 ($t1)

    bne  $t2 '*' end_term_left

    addi $t1 $t1 1

    addi $sp $sp -4
    sw   $ra 0($sp)
    jal  term
    move $t0 $v0        #right

    lw   $ra 0($sp)
    lw   $t1 4($sp)     #left
    addi $sp $sp 12

    mul  $t3 $t1 $t0
    move $v0 $t3

    jr   $ra

end_term_left:
    lw   $v0 4($sp)
    lw   $ra 0($sp)
    addi $sp $sp 8

    jr   $ra

number:
    li   $t0 0          #n = 0
    la   $t1 line       #*s
    lw   $t2 ($t1)      
loop_num:
    blt  $t2 '0' end_num
    bgt  $t2 '9' end_num

    mul  $t3 $t0 10
    sub  $t4 $t2 '0'
    add  $t0 $t3 $t4

    addi $t1 $t1 1
    j    loop_num

end_num:
    move $v0 $t0
    jr   $ra

.data
line:
    .space 10000

