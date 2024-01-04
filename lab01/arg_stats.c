#include <stdio.h>
#include <stdlib.h>

#define MAX_NUM 1000000

int main(int argc, char **argv) {
	(void) argc, (void) argv; // keep the compiler quiet, should be removed
	int min = MAX_NUM;
	int max = 0;
	int sum = 0;
	int prod = 1;
	int i = 1;
	int j = 0;
	int  num[MAX_NUM] = {0};
	//int atoi (const char **argv);
	while (i < argc) {
	    num[j] = atoi(argv[i]);
	    
	//min
	    if (min > num[j]) {
	        min = num[j];
	    }
	//max
	    if (max < num[j]) {
	        max = num[j];
	    }
	//sum
	    sum += num[j];
	//prod
	    prod = prod * num[j];
	    i++;
	    j++;
	}
	//mean
    int mean = sum/j;
    printf("MIN:  %d\nMAX:  %d\nSUM:  %d\nPROD: %d\nMEAN: %d\n",
        min, max, sum, prod, mean);

	return 0;
}
