# Files

## stdio.h

### main

#### fopen and fclose

```c
FILE *fopen(const char *pathname, const char *mode)
int fclose(FILE *stream)
```

###### example

```c
FILE *stream = fopen(pathname, "r")
fclose(stream)
```



#### read and write

```c
int fgetc(FILE *stream) // read a byte
int fputc(int c, FILE *stream) // write a byte
char *fputs(char *s, FILE *stream) // write a string
char *fgets(char *s, int size, FILE *stream) // read a line
int fscanf(FILE *stream, const char *format, ...)// formatted input
int fprintf(FILE *stream, const char *format, ...)// formatted output
```

###### example

```c
// create file "hello.txt" containing 1 line: Hello, Andrew
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[]) {
  FILE *output_stream = fopen("hello.txt", "w");
  if (output_stream == NULL) {
    perror("hello.txt");
    return 1;
  }
  fprintf(output_stream, "Hello, Andrew!\n");
  // fclose will flush data to file
  // best to close file ASAP
  // but doesn't matter as file autoamtically closed on exit
  fclose(output_stream);
  return 0;
}
```



### other

#### fseek

```c
int fseek(FILE *stream, long offset, int whence);
//whence can be SEEK_SET, SEEK_CUR or SEEK_END
```

###### example

```c
FILE *input_stream = fopen(argv[1], "rb");
// move to a position 1 byte from end of file
// then read 1 byte
fseek(input_stream, -1, SEEK_END);
printf("last byte of the file is 0x%02x\n", fgetc(input_stream));
// move to a position 0 bytes from start of file
// then read 1 byte
fseek(input_stream, 0, SEEK_SET);
printf("first byte of the file is 0x%02x\n", fgetc(input_stream));
// move to a position 41 bytes from start of file
// then read 1 byte
fseek(input_stream, 41, SEEK_SET);
printf("42nd byte of the file is 0x%02x\n", fgetc(input_stream));
// move to a position 58 bytes from current position
// then read 1 byte
fseek(input_stream, 58, SEEK_CUR);
printf("100th byte of the file is 0x%02x\n", fgetc(input_stream));
```



#### fflush

```c
int fflush(FILE *stream);
```



#### snprintf

- like printf, but output goes to char array str

```c
int snprintf(char *str, size_t size, const char *format, ...);
```



#### sscanf

- like scanf, but input comes from char array str

```c
int sscanf(const char *str, const char *format, ...);
```



#### stat

```c
#include <sys/stat.h>
int stat(const char *pathname, struct stat *statbuf)
```

###### struct stat

![Screen Shot 2020-11-26 at 15.07.23](/Users/a040/Library/Application Support/typora-user-images/Screen Shot 2020-11-26 at 15.07.23.png)

###### st_mode

- File type:

```c
S_IFMT
// type of file
S_IFBLK
// block special
S_IFCHR
// character special
S_IFIFO
// FIFO special
S_IFREG
// regular
S_IFDIR
// directory
S_IFLNK
// symbolic link
```

- File mode bits:

```c
S_IRWXU
// read, write, execute/search by owner
S_IRUSR
// read permission, owner
S_IWUSR
// write permission, owner
S_IXUSR
// execute/search permission, owner
S_IRWXG
// read, write, execute/search by group
S_IRGRP
// read permission, group
S_IWGRP
// write permission, group
S_IXGRP
// execute/search permission, group
S_IRWXO
// read, write, execute/search by others
S_IROTH
// read permission, others
S_IWOTH
// write permission, others
S_IXOTH
// execute/search permission, others
S_ISUID
// set-user-ID on execution
S_ISGID
// set-group-ID on execution
S_ISVTX
// on directories, restricted deletion flag
```

###### example

```c
struct stat s;
if (stat(pathname, &s) != 0) {
  perror(pathname);
  exit(1);
}
printf("ino = %10ld # Inode number\n", s.st_ino);
printf("mode = %10o # File mode \n", s.st_mode);
printf("nlink =%10ld # Link count \n", (long)s.st_nlink);
printf("uid = %10u # Owner uid\n", s.st_uid);
printf("gid = %10u # Group gid\n", s.st_gid);
printf("size = %10ld # File size (bytes)\n", (long)s.st_size);
printf("mtime =%10ld # Modification time (seconds since 1/1/70)\n", (long)s.st_mtime);

if(s.st_mode & S_IFDIR)  {
  printf( "%s is a directory\n", pathname);
}
```



#### other functions

##### - 失败的话等于0

##### mkdir

```C
int mkdir (const char *pathname, mode_t mode)
```

###### example

```C
#include <stdio.h>
#include <sys/stat.h>
// create the directories specified as command-line arguments
int main(int argc, char *argv[]) {
  for (int arg = 1; arg < argc; arg++) {
    if (mkdir(argv[arg], 0755) != 0) {
      perror(argv[arg]); // prints why the mkdir failed
      return 1;
    }
  }		
	return 0;
}
```



##### chmod

- hanging file permissions

```c
chmod(char *pathname, mode_t mode)
```

###### example

```c
// first argument is mode in octal
mode_t mode = strtol(argv[1], &end, 8);
// check first argument was a valid octal number
if (argv[1][0] == '\0' || end[0] != '\0') {
  fprintf(stderr, "%s: invalid mode: %s\n", argv[0], argv[1]);
  return 1;
}
for (int arg = 2; arg < argc; arg++) {
  if (chmod(argv[arg], mode) != 0) {
    perror(argv[arg]); // prints why the chmod failed
    return 1;
  }
}
```



##### unlink

- removing files

```c
unlink(char *pathname)
```



##### rename

- renaming a file

```c
rename(char *oldpath, char *newpath)
```



##### chdir

- change current working directory

```c
chdir(char *path)
```



##### getcwd

- get current working directory

```c
getcwd(char *buf, size_t size)
```



##### link

- create hard link to a file

```c
link(char *oldpath, char *newpath)
```



##### symlink

- create a symbolic link

```c
symlink(char *target, char *linkpath)
```



## POSIX functions to access directory contents (Advanced)

#### Directory

##### Open a directory stream

```c
#include <sys/types.h>
#include <dirent.h>
// open a directory stream for directory name
DIR *opendir(const char *name);
// return a pointer to next directory entry
struct dirent *readdir(DIR *dirp);
// close a directory stream
int closedir(DIR *dirp);
```

##### Print contents

```c
for (int arg = 1; arg < argc; arg++) {
  DIR *dirp = opendir(argv[arg]);
  if (dirp == NULL) {
  	perror(argv[arg]); // prints why the open failed
    return 1;
  }
  struct dirent *de;
  while ((de = readdir(dirp)) != NULL) {
    printf("%ld %s\n", de->d_ino, de->d_name);
  }
	closedir(dirp);
}
```































