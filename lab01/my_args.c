#include <stdio.h>
#include <string.h>

#define MAX_CH 1000
int main(int argc, char **argv) {
	(void) argc, (void) argv; // keep the compiler quiet, should be removed
	char input[MAX_CH] = {0};
	strcpy(input, *argv);
	printf("Program name: ");
	printf("%s\n", input);
	if (argc < 2) {
        printf("There are no other arguments\n");
    } else {
        printf("There are %d arguments:\n", argc - 1);
        int i = 0;
        char arg[MAX_CH] = {0};
        while (i < argc - 1) {
            strcpy(arg, argv[i + 1]);
            printf("	Argument %d is ", i + 1);
            printf("\"%s\"\n", arg);
            i++;
        }
    }
	return 0;
}
