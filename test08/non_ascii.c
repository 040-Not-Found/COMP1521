//non_ascii.c
//comp1521 test08 ex1
//z5311209

#include <stdio.h>

int main(int argc, char *argv[]) {

    char *filename = argv[1];
    FILE *stream = fopen(filename, "r");
    if (stream == NULL) {   //always check no errors
        perror(filename);
        return 1;
    }

    int c;
    int counter = 0;
    while ((c = fgetc(stream)) != EOF) {
        if (c >= 128 && c <= 255) {//is not ascii
            printf("%s: byte %d is non-ASCII\n", filename, counter);
            counter = -1;
            break;
        }
        counter++;
    }
    fclose(stream);

    if (counter != -1) {
        printf("%s is all ASCII\n", filename);
    }

    return 0;
}