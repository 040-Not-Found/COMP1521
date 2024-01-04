# mip cheat sheet.md

```c 
prinf("%d", x) 
```
```mips
    li $a0 $x
    li $v0 1
    syscall
```

```c
prinf("%c", x) 
```
```mips
    li $a0 $x
    li $v0 11
    syscall
```

```c
prinf("\n", x) 
```
```mips
    li $a0 '\n'
    li $v0 11
    syscall
```

```c
scanf("%d", &x)
```
```mips
    li $v0 5
    syscall
```

```c
return 0
```
```mips
    li $v0 0
    jr $ra
```