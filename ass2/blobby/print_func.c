#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[]) {

    char *filename = argv[1];
    FILE *stream = fopen(filename, "r");
    if (stream == NULL) {   //always check no errors
        perror(filename);
        return 1;
    }

    int c;
    while((c = fgetc(stream) ) != EOF) {
        printf("%d\n", c);
    }

    fclose(stream);
}    