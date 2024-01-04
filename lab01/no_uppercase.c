#include <stdio.h>
#include <ctype.h>

int main(void) {
    int ch;
    while ((ch = getchar()) != EOF) {
        if (ch >= 'A' && ch <= 'Z') {
        
            printf("%c", ch - 'A' + 'a');
        } else {
            printf("%c", ch);
        }
    }
	return 0;
}
