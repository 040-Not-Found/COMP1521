# pointer.s

main:
    la      $t0 answer  #p = &answer
    lw      $t1 ($t0)   #i = *p

    move    $a0 $t1     #print i
    li      $v0 1
    syscall

    li      $a0 '\n'
    li      $v0 11
    syscall

    li      $t2 27
    sw      $t2 ($t0)  

    lw      $a0 answer
    li      $v0 1
    syscall

    li      $a0 '\n'
    li      $v0 11
    syscall

    jr      $ra

.data
answer:
    .word 42