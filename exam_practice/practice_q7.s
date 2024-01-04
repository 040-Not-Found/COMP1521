# this code reads 1 integer and prints it
# change it to read integers until their sum is >= 42
# and then print theintgers read in reverse order

main:
    li   $t0 0         #i = 0
    li   $t1 0         #sum = 0
loop0:
    bgt  $t1 42 loop1

    la   $t2 numbers
    mul  $t3 $t0 4
    add  $t3 $t3 $t2

    li   $v0, 5        #   scanf("%d", &x);
    syscall            #

    sw   $v0 ($t3)
    
    addi  $t0 $t0 1
    add  $t1 $t1 $v0
    j    loop0

loop1:
    ble  $t0 0 end
    sub  $t0 $t0 1

    la   $t2 numbers
    mul  $t3 $t0 4
    add  $t3 $t3 $t2

    lw   $v0 ($t3)

    move $a0, $v0      #   printf("%d\n", x);
    li   $v0, 1
    syscall

    li   $a0, '\n'     #   printf("%c", '\n');
    li   $v0, 11
    syscall

    j    loop1

end:
    li   $v0, 0        # return 0
    jr   $ra

    .data
numbers:
    .space 4000