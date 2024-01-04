//practice_q5.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    char *envir1 = getenv(argv[1]);
    char *envir2 = getenv(argv[2]);

    if (envir1 == NULL || envir2 == NULL) {
        printf("0\n");
        return 0;
    }
    if (strcmp(envir1, envir2) == 0) {
        printf("1\n");
        return 1;
    }

    printf("0\n");
    return 0;
}