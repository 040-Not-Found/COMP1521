#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	(void) argc, (void) argv; // keep the compiler quiet, should be removed
	if (argc < 2) {
        printf("Usage: ./collatz NUMBER\n");
    } else {
    	int num = atoi(argv[1]);
    	printf("%d\n", num);
    	int f = 0;
    	while (num > 1) {
            if (num%2 == 0) {
                f = num/2;
            } else {
                f = 3*num + 1;
            }
            printf("%d\n", f);
            num = f;
        }
    }
	return EXIT_SUCCESS;
}
