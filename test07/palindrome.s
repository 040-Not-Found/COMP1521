# read a line and print whether it is a palindrom

main:
    la   $a0, str0       # printf("Enter a line of input: ");
    li   $v0, 4
    syscall

    la   $a0, line
    la   $a1, 256
    li   $v0, 8          # fgets(buffer, 256, stdin)
    syscall              #

    li   $t0 0          #i = 0
    la   $t1 line
loop0:
    add  $t2 $t0 $t1    #line[i]
    lb   $t2 ($t2)

    beq  $t2 0 end0
    
    addi $t0 $t0 1
    j    loop0
    
end0:
    li   $t1 0          #j
    sub  $t2 $t0 2      #k
    la   $t3 line
loop1:
    bge  $t1 $t2 result1

    add  $t4 $t1 $t3    #line[j]
    add  $t5 $t2 $t3    #line[k]
    lb   $t4 ($t4)
    lb   $t5 ($t5)

    bne  $t4 $t5 result0
    
    addi $t1 $t1 1
    sub $t2 $t2 1
    j    loop1

end1:
    li   $v0, 0          # return 0
    jr   $ra

result0:
    la   $a0, not_palindrome
    li   $v0, 4
    syscall

    j    end1

result1:
    la   $a0, palindrome
    li   $v0, 4
    syscall

    j    end1


.data
str0:
    .asciiz "Enter a line of input: "
palindrome:
    .asciiz "palindrome\n"
not_palindrome:
    .asciiz "not palindrome\n"


# line of input stored here
line:
    .space 256

