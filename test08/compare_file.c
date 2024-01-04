//compare_file.c
//comp1521 test08 ex1
//z5311209

#include <stdio.h>

int main(int argc, char *argv[]) {

    char *filename1 = argv[1];
    char *filename2 = argv[2];
    FILE *stream1 = fopen(filename1, "r");
    FILE *stream2 = fopen(filename2, "r");
    if (stream1 == NULL || stream2 == NULL) {   //always check no errors
        if (stream1 == NULL) {
            perror(filename1);
        } else {
            perror(filename2);
        }
        return 1;
    }

    int file1c = 0;
    int file2c = 0;
    int counter = 0;
    while ((file1c = fgetc(stream1)) != EOF && (file2c = fgetc(stream2)) != EOF) {
        if (file1c != file2c) {
            printf("Files differ at byte %d\n", counter);
            counter = -1;
            break;
        }
        counter++;   
    }
    file2c = fgetc(stream2);
    fclose(stream1);
    fclose(stream2);

    if (counter != -1) {
        if (file1c == EOF && file2c != EOF) {
        printf("EOF on %s\n", filename1);
        } else if (file1c != EOF && file2c == EOF) {
            printf("EOF on %s\n", filename2);
        } else {
            printf("Files are identical\n");
        } 
    }

    return 0;
}