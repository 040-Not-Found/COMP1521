// is_directory.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int main (int argc, char *argv[]) {
    char *pathname = argv[1];
    struct stat s;
    if(stat(pathname, &s) != 0 ) {
        printf("0\n");
        return 0;
    }
    if(s.st_mode & S_IFDIR)  {
        printf( "1\n");
        return 1;
    } else {
        printf("0\n");
        return 0;
    }
}