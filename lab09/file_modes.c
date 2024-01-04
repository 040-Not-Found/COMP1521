//file_modes.c
//z5311209

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

void print_mode(struct stat s);

int main (int argc, char *argv[]) {
    int i = 1;
    while (i < argc) {
        char *filename = argv[i];
        struct stat s;

        if (stat(filename, &s) != 0) {
            perror(filename);
            exit(1);
        }
        print_mode(s);
        
        printf(" %s\n", filename);
        i++;
    }

    return 0;
}

void print_mode(struct stat s) {
    
    if (S_ISDIR(s.st_mode)) {
        printf("d");
    } else {
        printf("-");
    }
    if (s.st_mode & S_IRUSR) {
        printf("r");
    } else {
        printf("-");
    }
    if (s.st_mode & S_IWUSR) {
        printf("w");
    } else {
        printf("-");
    }
    if (s.st_mode & S_IXUSR) {
        printf("x");
    } else {
        printf("-");
    }
    if (s.st_mode & S_IRGRP) {
        printf("r");
    } else {
        printf("-");
    }
    if (s.st_mode & S_IWGRP) {
        printf("w");
    } else {
        printf("-");
    }
    if (s.st_mode & S_IXGRP) {
        printf("x");
    } else {
        printf("-");
    }
    if (s.st_mode & S_IROTH) {
        printf("r");
    } else {
        printf("-");
    }
    if (s.st_mode & S_IWOTH) {
        printf("w");
    } else {
        printf("-");
    }
    if (s.st_mode & S_IXOTH) {
        printf("x");
    } else {
        printf("-");
    }
}