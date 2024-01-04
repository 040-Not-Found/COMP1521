# Sieve of Eratosthenes
# https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes
main:
    li   $t0 0          #i = 0
    la   $t1 prime
loop0:
    bge  $t0 1000 end0

    mul  $t2 $t0 4
    add  $t3 $t2 $t1    #addr
    
    li   $t4 1
    sw   $t4 ($t3)      #prime[i] = 1

    addi $t0, $t0, 1	#i++
    j loop0

end0:
    li   $t0 2          #i = 2
loop1:
    bge  $t0 1000 end

    mul  $t2 $t0 4
    add  $t3 $t2 $t1    #addr
    lw   $t4 ($t3)      #prime[i]
    
    beq  $t4 1 if_statement     #if (prime[i] == 1)

    j end1


if_statement:
    move $a0 $t0        #   printf("%d", i)
    li   $v0, 1        
    syscall

    li   $a0, '\n'      #   printf("%c", '\n');
    li   $v0, 11
    syscall

    mul  $t5 $t0 2      #j = 2 * i

loop2:
    bge  $t5 1000 end1

    li   $t6 0

    mul  $t2 $t5 4
    add  $t3 $t2 $t1    #addr
    sw   $t6 ($t3)

    add $t5, $t5, $t0	#i++
    j loop2

end1:
    addi $t0, $t0, 1	#i++
    j loop1

end:
    li $v0, 0           # return 0
    jr $31

.data
prime:
    .space 1000