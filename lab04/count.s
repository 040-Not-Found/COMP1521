# read a number n and print the integers 1..n one per line

main:                 # int main(void)
    la   $a0, prompt  # printf("Enter a number: ");
    li   $v0, 4
    syscall

    li   $v0, 5       # scanf("%d", number);
    syscall
    move $t0, $v0
    
    li   $t1, 1         # i = 1
    
loop:
    bge  $t1, $t0, end
    move $a0, $t1      # printf("%d", i);
    li   $v0, 1
    syscall

    li   $a0, '\n'    # printf("%c", '\n');
    li   $v0, 11
    syscall
    
    addi $t1, $t1, 1    # i = i + 1;
    j    loop
    syscall
    

end:
    move $a0, $t0
    li   $v0, 1
    syscall
    
    li   $a0, '\n'    # printf("%c", '\n');
    li   $v0, 11
    syscall
    
    jr   $ra          # return

    .data
prompt:
    .asciiz "Enter a number: "
