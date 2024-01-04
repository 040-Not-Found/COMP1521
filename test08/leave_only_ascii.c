//non_ascii.c
//comp1521 test08 ex1
//z5311209

#include <stdio.h>

int main(int argc, char *argv[]) {

    char *filename = argv[1];
    FILE *stream = fopen(filename, "r");
    FILE *copy_stream = fopen("copy", "w");
    if (stream == NULL) {   //always check no errors
        perror(filename);
        return 1;
    }
    if (copy_stream == NULL) {   //always check no errors
        perror("copy");
        return 1;
    }

    int c;
    int counter = 0;
    while ((c = fgetc(stream)) != EOF) {
        if (c < 128 || c > 255) {//is ascii
            fputc(c, copy_stream);
        }
        counter++;
    }
    fclose(stream);
    remove(filename);
    rename("copy", filename);

    return 0;
}