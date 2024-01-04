//file_sizes.c
//z5311209

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main (int argc, char *argv[]) {
    int i = 1;
    long total = 0;
    while (i < argc) {
        char *filename = argv[i];
        struct stat s;

        if (stat(filename, &s) != 0) {
            perror(filename);
            exit(1);
        }

        printf("%s: %ld bytes\n", filename, (long)s.st_size);

        total += (long)s.st_size;

        i++;
    }
    printf("Total: %ld bytes\n", total);

    return 0;
}