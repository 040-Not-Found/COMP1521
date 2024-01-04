# Read 10 numbers into an array
# print 0 if they are in non-decreasing order
# print 1 otherwise

# i in register $t0

main:

    li   $t0, 0         # i = 0
loop0:
    bge  $t0, 10, end0  # while (i < 10) {

    li   $v0, 5         #   scanf("%d", &numbers[i]);
    syscall             #

    mul  $t1, $t0, 4    #   calculate &numbers[i]
    la   $t2, numbers   #
    add  $t3, $t1, $t2  #
    sw   $v0, ($t3)     #   store entered number in array

    addi $t0, $t0, 1    #   i++;
    j    loop0          # }
end0:

    li   $t0, 0         #i = 0
    li   $t1, 1         #j = 1
loop1:

    bgt  $t0, 8, result0  #i < 10

    mul  $t3, $t0, 4    
    mul  $t4, $t1, 4    
    la   $t2, numbers
    add  $t5, $t2, $t3  #y address
    add  $t6, $t2, $t4  #x address

    lw   $t7, ($t5)     #y
    lw   $t8, ($t6)     #x

    blt  $t8, $t7, result1

    addi $t0, $t0, 1    #   i++;
    addi $t1, $t1, 1    #   j++;
    j    loop1

result0:
    li   $a0, 0         # printf("%d", 0)
    li   $v0, 1         #
    syscall

    j    end1

result1:
    li   $a0, 1         # printf("%d", 1)
    li   $v0, 1         #
    syscall

    j    end1

end1:
    li   $a0, '\n'      # printf("%c", '\n');
    li   $v0, 11
    syscall

    jr   $ra

.data

numbers:
    .word 0 0 0 0 0 0 0 0 0 0  # int numbers[10] = {0};
