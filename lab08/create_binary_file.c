#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    char *filename = argv[1];
    FILE *stream = fopen(filename, "w");
    if (stream == NULL) {   //always check no errors
        perror(filename);
        return 1;
    }

    int i = 2;
    int j = atoi(argv[i]);
    while (i < argc) {
        j = atoi(argv[i]);
        fputc(j, stream);
        i++;
    }
    fclose(stream);
    return 0;
}