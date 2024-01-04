//is_var_set.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char *argv[]) {
    char *environment = NULL;
    if (argc == 2) {
        environment = argv[1];
    } else if (argc == 3) {
        environment = argv[2];
    }
    char *home = getenv(environment);
    //printf("%s\n", home);
    
    if (home != NULL) {
        FILE *p = fopen(home, "r");
        if (p != NULL) {
            printf("1\n");
            return 1;
        } else if (strcmp(home, "") == 1) {
            printf("1\n");
            return 1;
        }
    }
    printf("0\n");
    return 0;
}