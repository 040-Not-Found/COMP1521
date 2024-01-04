# read 10 numbers into an array
# bubblesort them
# then print the 10 numbers

# i in register $t0
# registers $t1, $t2 & $t3 used to hold temporary results

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
    li   $t9, 1         #swapped = 1

loop1:
    beq  $t9, 0, end2 #while(swapped != 0)

    li   $t9, 0         #swapped = 0
    li   $t0, 0         #i = 0
    li   $t1, 1         #j = 1
loop2:
    bgt  $t0, 8, loop1  #i < 8

    mul  $t3, $t0, 4    
    mul  $t4, $t1, 4    
    la   $t2, numbers
    add  $t5, $t2, $t3  #y address
    add  $t6, $t2, $t4  #x address

    lw   $t7, ($t5)     #y
    lw   $t8, ($t6)     #x
    
    blt  $t8, $t7, swap #if x < y

    addi $t0, $t0, 1    #   i++;
    addi $t1, $t1, 1    #   j++;
    j    loop2

    
swap:
    li   $t9, 1         #swapped = 1
    sw   $t7, ($t6)     #y=x
    sw   $t8, ($t5)     #x=y

    addi $t0, $t0, 1    #   i++;
    addi $t1, $t1, 1    #   j++;
    j    loop2

end2:

    li   $t0, 0         # i = 0
print_loop:
    bge  $t0, 10, end1  # while (i < 10) {

    mul  $t1, $t0, 4    #   calculate &numbers[i]
    la   $t2, numbers   #
    add  $t3, $t1, $t2  #
    lw   $a0, ($t3)     #   load numbers[i] into $a0
    li   $v0, 1         #   printf("%d", numbers[i])
    syscall

    li   $a0, '\n'      #   printf("%c", '\n');
    li   $v0, 11
    syscall

    addi $t0, $t0, 1    #   i++
    j    print_loop     # }

end1:
    li   $v0, 0
    jr   $ra            # return 0

.data

numbers:
    .word 0 0 0 0 0 0 0 0 0 0  # int numbers[10] = {0};



