// practice_q6.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    char *filename = argv[1];
    FILE *stream = fopen(filename, "r");

    if (stream == NULL) {
        perror(filename);
        exit(1);
    }

    int c;
    int counter = 0;
    while ((c = fgetc(stream)) != EOF) {
        if (c >= 0 && c <= 127) {
            counter++;
        }
    }

    printf("%s contains %d ASCII bytes\n", filename, counter);
    return 0;
}