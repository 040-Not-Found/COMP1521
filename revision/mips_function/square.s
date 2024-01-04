#square.s
main:
    addi    $sp $sp -40
    li      $t0 0

loop0:
    bge     $t0 10 end0

    mul     $t1 $t0 $t0
    mul     $t2 $t0 4
    add     $t2 $t2 $sp
    sw      $t1 ($t2)

    addi    $t0 $t0 1
    j		loop0				# jump to loop0

end0:
    li      $t0 0
loop1:
    bge     $t0 10 end1

    mul     $t1 $t0 $t0
    mul     $t2 $t0 4
    add     $t2 $t2 $sp
    lb      $t1 ($t2)

    move    $a0 $t1
    li      $v0 1
    syscall

    li      $a0 '\n'
    li      $v0 11
    syscall

    addi    $t0 $t0 1
    j		loop1

end1:
    addi    $sp $sp 40
    li      $v0 0
    jr      $ra


    
