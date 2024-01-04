# Read a number and print positive multiples of 7 or 11 < n

main:                  # int main(void) {

    la   $a0, prompt   # printf("Enter a number: ");
    li   $v0, 4
    syscall

    li   $v0, 5         # scanf("%d", number);
    syscall
    move $t0, $v0
    
    li   $t1, 1         # i = 1
    
loop:
    bge  $t1, $t0, end
    
    #mod 7
    rem  $t7, $t1, 7
    
    #mod 11
    rem $t9, $t1, 11
    
    #if statements
    beq $t7, 0, print
    beq $t9, 0, print
    
    addi $t1, $t1, 1    # i = i + 1;
    j    loop
    
    
print:
    move $a0, $t1        #   printf("%d", $t1);
    li   $v0, 1
    syscall

    li   $a0, '\n'      # printf("%c", '\n');
    li   $v0, 11
    syscall
    
next_loop: 
    addi $t1, $t1, 1    # i = i + 1;
    j    loop
    
end:
    jr   $ra           # return

    .data
prompt:
    .asciiz "Enter a number: "
