#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    char *filename = argv[1];
    FILE *stream = fopen(filename, "w");
    if (stream == NULL) {   //always check no errors
        perror(filename);
        return 1;
    }

    int i = atoi(argv[2]);
    int j = atoi(argv[3]);
    while (i <= j) {
        fprintf(stream, "%d\n", i);
        i++;
    }
    fclose(stream);
    return 0;
}