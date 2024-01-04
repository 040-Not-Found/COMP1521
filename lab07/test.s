main:
    jal hello
    jal hello
    j end

end:
    li $v0 0
    jr $ra

hello:
    la $a0 string
    li $v0 4
    syscall
    jr $ra


.data
string: .asciiz "hello\n"