# read a mark and print the corresponding UNSW grade

main:
    la   $a0, prompt    # printf("Enter a mark: ");
    li   $v0, 4
    syscall

    li   $v0, 5         # scanf("%d", mark);
    syscall
    move $t0, $v0
    
    blt $t0, 50, FL

    blt $t0, 65, PS
    
    blt $t0, 75, CR

    blt $t0, 85, DN

    ble $t0, 100, HD
    
FL: 
    la   $a0, fl        # printf("FL\n");
    li   $v0, 4
    syscall
    j    end
CR: 
    la   $a0, cr        # printf("PS\n");
    li   $v0, 4
    syscall
    j    end
PS: 
    la   $a0, ps        # printf("PS\n");
    li   $v0, 4
    syscall
    j    end
DN: 
    la   $a0, dn        # printf("DN\n");
    li   $v0, 4
    syscall
    j    end
HD: 
    la   $a0, hd        # printf("HD\n");
    li   $v0, 4
    syscall
    j    end

end:
    jr   $ra            # return

    .data
prompt:
    .asciiz "Enter a mark: "
fl:
    .asciiz "FL\n"
ps:
    .asciiz "PS\n"
cr:
    .asciiz "CR\n"
dn:
    .asciiz "DN\n"
hd:
    .asciiz "HD\n"
