# Unicode(UTF-8)

![Screen Shot 2020-11-26 at 15.56.46](/Users/a040/Library/Application Support/typora-user-images/Screen Shot 2020-11-26 at 15.56.46.png)

###### example

```c
printf("The unicode code point U+1F600 encodes in UTF-8\n");
printf("as 4 bytes: 0xF0 0x9F 0x98 0x80\n");
printf("We can output the 4 bytes like this: \xF0\x9F\x98\x80\n");
printf("Or like this: ");
putchar(0xF0);
putchar(0x9F);
putchar(0x98);
putchar(0x80);
putchar('\n');
```

