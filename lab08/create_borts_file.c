#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[]) {

    char *filename = argv[1];
    FILE *stream = fopen(filename, "w");
    if (stream == NULL) {   //always check no errors
        perror(filename);
        return 1;
    }

    uint16_t i = atoi(argv[2]);
    uint16_t j = atoi(argv[3]);
    uint8_t x;
    uint8_t y;
    while (i <= j) {
        x = i >> 8;
        x = x & 0xFF;
        y = i & 0xFF;
        fputc(x, stream);
        fputc(y, stream);
        i++;
    }

    fclose(stream);
    return 0;
}