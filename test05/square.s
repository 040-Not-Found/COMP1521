main:
    li   $v0, 5         #   scanf("%d", &x);
    syscall             #
    move $t0, $v0

    li   $t1, 0         #i = 0
loop0:
    bge  $t1, $t0, end  #while i < x
    li   $t2, 0         #j = 0
loop1:
    bge  $t2, $t0, skip #while j < x
    li   $v0, 4
    la   $a0, star
    syscall
    addi $t2, $t2, 1    #j++
    j    loop1
skip:
    li   $a0, '\n'      #   printf("%c", '\n');
    li   $v0, 11
    syscall

    addi $t1, $t1, 1    #j++
    j    loop0


end:

    li   $v0, 0         # return 0
    jr   $ra


    .data
star: .asciiz  "*"
