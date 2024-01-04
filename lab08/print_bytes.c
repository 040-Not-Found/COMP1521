#include <stdio.h>

int main(int argc, char *argv[]) {

    char *filename = argv[1];
    FILE *stream = fopen(filename, "r");
    if (stream == NULL) {   //always check no errors
        perror(filename);
        return 1;
    }
    
    int c = fgetc(stream);
    int i = 0;
    while (c != EOF) {
        if (i < 10) {
            if ((c >= 'a' && c <= 'z') || c == ' ') {
                printf("byte    %d: %3d 0x%02x '%c'\n", i, c, c, c);
            } else {
                printf("byte    %d: %3d 0x%02x \n", i, c, c);
            }
        } else {
            if ((c >= 'a' && c <= 'z') || c == ' ') {
                printf("byte   %d: %3d 0x%02x '%c'\n", i, c, c, c);
            } else {
                printf("byte   %d: %3d 0x%02x \n", i, c, c);
            }
        }

        c = fgetc(stream);
        i++;
    }
    fclose(stream);
    return 0;
}