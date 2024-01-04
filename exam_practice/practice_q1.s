#practice_q1.s

main:
    li $v0 5
    syscall

    move $t0 $v0            #x

    li $v0 5
    syscall

    move $t1 $v0            #y

    add $t2 $t1 $t0

    move $a0 $t2
    li $v0 1
    syscall

    li $a0 '\n'
    li $v0 11
    syscall

    li   $v0, 0             # return 0
    jr   $ra
