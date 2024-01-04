#two_powerful.s

main:
    addi    $sp $sp, -4
    sw      $ra 0($sp)		

    li      $a0 1		#i
    jal     two

    lw      $ra 0($sp)
    addi    $sp $sp 4

    jr      $ra

two:
    addi    $sp $sp -8
    sw      $ra 4($sp)
    sw      $a0 0($sp)

    bge     $a0 1000000 print_func
    mul     $a0 $a0 2
    jal     two

print_func:
    lw      $a0 0($sp)
    li      $v0 1       #printf("%d", i);
    syscall

    li      $a0 '\n'    #printf("\n");
    li      $v0 11
    syscall

    lw      $ra 4($sp)
    addi    $sp $sp 8

    jr      $ra
