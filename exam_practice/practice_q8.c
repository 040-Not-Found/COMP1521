// practice_q8.c

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define TWO_BYTES 49280
#define THREE_BYTES 12615808
#define FOUR_BYTES 3229646976

int check(int c, char *pathname, int counter);

int main (int argc, char *argv[]) {
    char *pathname = argv[1];
    FILE *stream = fopen(pathname, "r");

    if (stream == NULL) {
        perror(pathname);
        exit(1);
    }
    int c;
    int counter = 0;
    int check_counter;
    uint8_t byte_1;
    uint8_t byte_2;
    uint8_t byte_3;
    uint8_t byte_4;
    while ((c = fgetc(stream)) != EOF) {
        check_counter = 0;
        byte_1 = c >> 7;
        byte_2 = c >> 5;
        byte_3 = c >> 4;
        byte_4 = c >> 3;
        if (byte_1 == 0) {
            counter++;
        } else if (byte_2 == 6) {
            c = fgetc(stream);
            check_counter += check(c, pathname, counter);
            if (check_counter == 1) {
                counter++;
            }
        } else if (byte_3 == 14) {
            for (int i = 0; i < 2; i++) {
                c = fgetc(stream);
                check_counter += check(c, pathname, counter);
            }
            if (check_counter == 2) {
                counter++;
            }
        } else if (byte_4 == 30) {
            for (int i = 0; i < 3; i++) {
                c = fgetc(stream);
                check_counter += check(c, pathname, counter);

            }
            if (check_counter == 3) {
                counter++;
            }
        }
    }
    printf("%s: %d UTF-8 characters\n", pathname, counter);

    return 0;
}

int check(int c, char *pathname, int counter) {
    uint8_t byte = c >> 6;
    if (byte == 2) {
        return 1;
    }
    printf("%s: invalid UTF-8 after %d valid UTF-8 characters\n", pathname, counter);
    exit(1);
}