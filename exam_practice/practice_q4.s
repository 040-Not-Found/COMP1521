# this code reads 1 integer and prints it
# change it to read integers until their sum is >= 42
# and then print their sum

main:
    li $t0 0            #x
    li $t1 0            #count
    li $t2 0            #sum
loop:
    bge $t0 100 end
    add $t2 $t2 $t0
    bge $t2 42 end1

    ble $t0 10 if_func

keep:
    li   $v0, 5         #   scanf("%d", &x);
    syscall             #
    move $t0 $v0

    j loop			    # jump to loop
    
if_func:
    addi $t1 $t1 1
    j keep
    
end:
    move $a0, $t0      #   printf("%d\n", x);
    li   $v0, 1
    syscall

    li   $a0, '\n'     #   printf("%c", '\n');
    li   $v0, 11
    syscall

    li   $v0, 0        # return 0
    jr   $ra

end1:
    move $a0, $t2      #   printf("%d\n", x);
    li   $v0, 1
    syscall

    li   $a0, '\n'     #   printf("%c", '\n');
    li   $v0, 11
    syscall

    li   $v0, 0        # return 0
    jr   $ra
