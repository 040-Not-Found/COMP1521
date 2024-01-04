#include <stdio.h>
#include <stdlib.h>

#define SERIES_MAX 46

int function(int num);

int main(void) {
    int already_computed[SERIES_MAX + 1] = { 0, 1 };
	(void) already_computed; // keep the compiler quiet, should be removed
	int num, f = 0;
    while ((scanf("%d", &num)) != EOF) {
        f = function(num);
        printf("%d\n", f);
    }
    return EXIT_SUCCESS;
}

int function(int num) {
    if (num <= 1) {
        return num;
    }
    return function(num - 1) + function(num - 2);
}
