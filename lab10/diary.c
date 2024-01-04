//z5311209

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char *argv[]) {
    char *home = getenv("HOME");
    char *diary = "/.diary";
    char *pathname = malloc(strlen(home) + strlen(diary) + 1);
    strcpy(pathname, home);
    strcat(pathname, diary);

    FILE *stream = fopen(pathname, "a");

    if (stream == NULL) {
        perror(pathname);
        exit(1);
    }

    int i = 1;
    while (i < argc) {
        if (i == argc - 1) {
            fprintf(stream, "%s\n", argv[i]);
        } else {
            fprintf(stream, "%s ", argv[i]);
        }
        i++;
    }
    fclose(stream);
    return 0;
}