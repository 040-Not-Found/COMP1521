//is_diary_public.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int main (int argc, char *argv[]) {
    char *home = getenv("HOME");
    char *diary = "/.diary";
    char *pathname = malloc(strlen(home) + strlen(diary) + 1);
    strcpy(pathname, home);
    strcat(pathname, diary);

    FILE *stream = fopen(pathname, "r");
    if (stream == NULL) {
        printf("0\n");
        return 0;   
    }

    struct stat s;
    if (stat(pathname, &s) != 0) {
        printf("0\n");
        return 0; 
    }
    if (s.st_mode & S_IROTH) {
        printf("1\n");
        return 1;
    }
    printf("0\n");
    return 0;

}