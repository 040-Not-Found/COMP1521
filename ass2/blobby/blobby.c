// blobby.c
// blob file archiver
// COMP1521 20T3 Assignment 2
// Written by <Celine Lin z5311209>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

// the first byte of every blobette has this value
#define BLOBETTE_MAGIC_NUMBER          0x42

// number of bytes in fixed-length blobette fields
#define BLOBETTE_MAGIC_NUMBER_BYTES    1
#define BLOBETTE_MODE_LENGTH_BYTES     3
#define BLOBETTE_PATHNAME_LENGTH_BYTES 2
#define BLOBETTE_CONTENT_LENGTH_BYTES  6
#define BLOBETTE_HASH_BYTES            1

// maximum number of bytes in variable-length blobette fields
#define BLOBETTE_MAX_PATHNAME_LENGTH   65535
#define BLOBETTE_MAX_CONTENT_LENGTH    281474976710655


// ADD YOUR #defines HERE


typedef enum action {
    a_invalid,
    a_list,
    a_extract,
    a_create
} action_t;


void usage(char *myname);
action_t process_arguments(int argc, char *argv[], char **blob_pathname,
                           char ***pathnames, int *compress_blob);

void list_blob(char *blob_pathname);
void extract_blob(char *blob_pathname);
void create_blob(char *blob_pathname, char *pathnames[], int compress_blob);

uint8_t blobby_hash(uint8_t hash, uint8_t byte);


// ADD YOUR FUNCTION PROTOTYPES HERE
void find_details(FILE *stream, int c, long *mode,
                  int *pathname_length, long long *content_length, 
                  int *hash);
char *find_pathname(FILE *stream, int pathname_length, int *hash);
char *find_content(FILE *stream, int content_length, int *hash);
long find_mode(int mode_array[]);
int find_pathname_length(int pathname_length_array[]);
long long find_content_length(long long content_length_array[]);
void put_mode(mode_t st_mode, FILE *new_blob, int *hash);
void put_pathname_length(FILE *new_blob, int pathname_length, int *hash);
void put_content_length(size_t st_size, FILE *new_blob, int *hash);
// YOU SHOULD NOT NEED TO CHANGE main, usage or process_arguments

int main(int argc, char *argv[]) {
    char *blob_pathname = NULL;
    char **pathnames = NULL;
    int compress_blob = 0;
    action_t action = process_arguments(argc, argv, &blob_pathname, &pathnames,
                                        &compress_blob);

    switch (action) {
    case a_list:
        list_blob(blob_pathname);
        break;

    case a_extract:
        extract_blob(blob_pathname);
        break;

    case a_create:
        create_blob(blob_pathname, pathnames, compress_blob);
        break;

    default:
        usage(argv[0]);
    }

    return 0;
}

// print a usage message and exit

void usage(char *myname) {
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "\t%s -l <blob-file>\n", myname);
    fprintf(stderr, "\t%s -x <blob-file>\n", myname);
    fprintf(stderr, "\t%s [-z] -c <blob-file> pathnames [...]\n", myname);
    exit(1);
}

// process command-line arguments
// check we have a valid set of arguments
// and return appropriate action
// **blob_pathname set to pathname_array for blobfile
// ***pathname_array set to a list of pathnames for the create action
// *compress_blob set to an integer for create action

action_t process_arguments(int argc, char *argv[], char **blob_pathname,
                           char ***pathnames, int *compress_blob) {
    extern char *optarg;
    extern int optind, optopt;
    int create_blob_flag = 0;
    int extract_blob_flag = 0;
    int list_blob_flag = 0;
    int opt;
    while ((opt = getopt(argc, argv, ":l:c:x:z")) != -1) {
        switch (opt) {
        case 'c':
            create_blob_flag++;
            *blob_pathname = optarg;
            break;

        case 'x':
            extract_blob_flag++;
            *blob_pathname = optarg;
            break;

        case 'l':
            list_blob_flag++;
            *blob_pathname = optarg;
            break;

        case 'z':
            (*compress_blob)++;
            break;

        default:
            return a_invalid;
        }
    }

    if (create_blob_flag + extract_blob_flag + list_blob_flag != 1) {
        return a_invalid;
    }

    if (list_blob_flag && argv[optind] == NULL) {
        return a_list;
    } else if (extract_blob_flag && argv[optind] == NULL) {
        return a_extract;
    } else if (create_blob_flag && argv[optind] != NULL) {
        *pathnames = &argv[optind];
        return a_create;
    }

    return a_invalid;
}


// list the contents of blob_pathname

void list_blob(char *blob_pathname) {
    FILE *stream = fopen(blob_pathname, "r");

    if (stream == NULL) {//check error
        perror(blob_pathname);
        exit(1);
    }

    int c;
    int counter;
    long mode = 0;
    int pathname_length = 0;
    long long content_length = 0;
    char *pathname;
    int hash = 0;
    while ((c = fgetc(stream)) != EOF) {
        //find the information about the files
        //in the blob file
        find_details(stream, c, &mode, &pathname_length, &content_length, &hash);
        pathname = find_pathname(stream, pathname_length, &hash);

        printf("%06lo %5lu %s\n", (long)mode, (long)content_length, pathname);
        
        //loop thougth until the next file
        counter = 0;
        while (counter < (content_length + 1)) {
            c = fgetc(stream);
            counter++;
        }

        //when finding the pathname
        //the find_pathname had open
        //a malloc to store the pathname
        free(pathname);
    }
    fclose(stream);
}

// extract the contents of blob_pathname

void extract_blob(char *blob_pathname) {
    FILE *input_stream = fopen(blob_pathname, "r");
    FILE *output_stream = NULL;
    
    if (input_stream == NULL) {//check error
        perror(blob_pathname);
        exit(1);
    }

    int c;
    int counter = 0;
    long input_mode = 0;
    int pathname_length = 0;
    long long content_length = 0;
    char *pathname;
    char *content;
    int hash = 0;
    while ((c = fgetc(input_stream)) != EOF) {
        hash = 0;
        hash = blobby_hash(hash, c);

        //seperate the informations in the blob file
        find_details(input_stream, c, &input_mode, &pathname_length, &content_length, &hash);
        pathname = find_pathname(input_stream, pathname_length, &hash);

        //extract the content of the code
        printf("Extracting: %s\n", pathname);
        content = find_content(input_stream, content_length, &hash);

        //create a new file
        output_stream = fopen(pathname, "w");
        
        if (output_stream == NULL) {// check error
            perror(pathname);
            exit(1);
        }

        //set the mode/permission
        mode_t output_mode = input_mode & ~S_IWOTH;

        if (chmod(pathname, output_mode) != 0) {
            perror(pathname);
            exit(1);
        }

        //put in the content
        counter = 0;
        while (counter < content_length) {
            fputc(content[counter], output_stream);
            counter++;
        }

        //this is the hash in the file
        //if the hash in the file does not
        //equal to the hash calculated above
        //exit the function with error
        c = fgetc(input_stream);
        if (hash != c) {
            fprintf(stderr, "ERROR: blob hash incorrect\n");
            exit(1);
        }

        free(pathname);
        free(content);
        fclose(output_stream);
    }
    fclose(input_stream);
}

// create blob_pathname from NULL-terminated array pathnames
// compress with xz if compress_blob non-zero (subset 4)

void create_blob(char *blob_pathname, char *pathnames[], int compress_blob) {

    FILE *new_blob = fopen(blob_pathname, "a");

    if (new_blob == NULL) {//check error
        perror(blob_pathname);
        exit(1);
    }

    FILE *file = NULL;
    int c;
    int counter;
    int pathname_length = 0;
    int hash;
    for (int p = 0; pathnames[p]; p++) {
        hash = 0;
        printf("Adding: %s\n", pathnames[p]);
        pathname_length = strlen(pathnames[p]);

        file = fopen(pathnames[p], "r");

        if (file == NULL) {//check error
            perror(pathnames[p]);
            exit(1);
        }
        
        //put magic num
        fputc(BLOBETTE_MAGIC_NUMBER, new_blob);
        hash = blobby_hash(hash, BLOBETTE_MAGIC_NUMBER);
        
        struct stat s;
        
        if (stat(pathnames[p], &s) != 0) {
            perror(pathnames[p]);
            exit(1);
        }

        put_mode(s.st_mode, new_blob, &hash);
        
        put_pathname_length(new_blob, pathname_length, &hash);

        put_content_length(s.st_size, new_blob, &hash);

        //put pathname
        counter = 0;
        while (counter < pathname_length) {
            hash = blobby_hash(hash, pathnames[p][counter]);
            fputc(pathnames[p][counter], new_blob);
            counter++;
        }
        

        //put content
        counter = 0;
        while ((c = fgetc(file)) != EOF) {
            hash = blobby_hash(hash, c);
            fputc(c, new_blob);
            counter++;
        }

        //put_hash
        fputc(hash, new_blob);

        fclose(file);
    }
    fclose(new_blob);

}


// ADD YOUR FUNCTIONS HERE
void find_details(FILE *stream, int c, long *mode,
                  int *pathname_length, long long *content_length, 
                  int *hash) {
    int byte_counter = 1;
    int mode_counter = 0;
    int mode_array[BLOBETTE_MODE_LENGTH_BYTES];
    int pathname_length_counter = 0;
    int pathname_length_array[BLOBETTE_PATHNAME_LENGTH_BYTES];
    int content_length_counter = 0;
    long long content_length_array[BLOBETTE_CONTENT_LENGTH_BYTES];

    //if the first byte is not 'B', the magic number
    //exit the function with error
    if (c != BLOBETTE_MAGIC_NUMBER) {
            fprintf(stderr, "ERROR: Magic byte of blobette incorrect\n");
            exit(1);
    }

    //as byte 1 to byte 11 are stored for
    //mode, pathname length and content length, 
    //so the loop function will only loop for 
    //11 times
    //(byte 0 is already checked)
    while (byte_counter < 12 && (c = fgetc(stream)) != EOF) {
        //find the hash after fgetc
        *hash = blobby_hash(*hash, c);

        //the byte that stores the mode
        if (byte_counter >= 1 && byte_counter <= 3) {
            mode_array[mode_counter] = c;
            mode_counter++;

            //put the parts of the mode together
            // by the "find_mode" function
            if (byte_counter == 3) {
                *mode = find_mode(mode_array);
            }

        //the functions below are doing the similar things
        } else if (byte_counter >= 4 && byte_counter <= 5){
            pathname_length_array[pathname_length_counter] = c;
            pathname_length_counter++;

            if (byte_counter == 5) {
                *pathname_length = find_pathname_length(pathname_length_array);
            }
        } else if (byte_counter >= 6 && byte_counter <= 11){
            content_length_array[content_length_counter] = c;
            content_length_counter++;

            if (byte_counter == 11) {
                *content_length = find_content_length(content_length_array);
            }
        }
        byte_counter++;
    }

}

long find_mode(int mode_array[]) {
    //put the separated bytes of mode together
    //and return the mode
    int i = 0;
    int j = BLOBETTE_MODE_LENGTH_BYTES - 1;
    long mode;
    long return_mode = 0;
    while (i < BLOBETTE_MODE_LENGTH_BYTES) {
        mode = mode_array[i] << (8*j);
        return_mode += mode;
        i++;
        j--;
    }
    return return_mode;
}

int find_pathname_length(int pathname_length_array[]) {
    //put the separated bytes of pathname length together
    //and return the pathnmae length
    int i = 0;
    int j = BLOBETTE_PATHNAME_LENGTH_BYTES - 1;
    int pathname_length;
    int return_pathname_length = 0;
    while (i < BLOBETTE_PATHNAME_LENGTH_BYTES) {
        pathname_length = pathname_length_array[i] << (8*j);
        return_pathname_length += pathname_length;
        i++;
        j--;
    }
    return return_pathname_length;
}

long long find_content_length (long long content_length_array[]) {
    //put the separated bytes of content length together
    //and return the content length
    int i = 0;
    int j = BLOBETTE_CONTENT_LENGTH_BYTES - 1;
    long long content_length;
    long return_content_length = 0;
    while (i < BLOBETTE_CONTENT_LENGTH_BYTES) {
        content_length = content_length_array[i] << (8*j);
        return_content_length += content_length;
        i++;
        j--;
    }
    return return_content_length;
}

char *find_pathname(FILE *stream, int pathname_length, int *hash) {
    int counter = 0;
    int c;
    //open a malloc to store the pathname
    char *pathname_array = malloc(sizeof(char) * (pathname_length + 1));

    //put the pathname in the array
    while (counter < pathname_length && (c = fgetc(stream)) != EOF) {
        //find the hash after fgetc
        *hash = blobby_hash(*hash, c);

        pathname_array[counter] = c;
        counter++;
    }
    //the last element is a string is '\0'
    //and return the pathname string
    pathname_array[counter] = '\0';
    char *pathname = pathname_array;
    return pathname;
}

char *find_content(FILE *stream, int content_length, int *hash) {
    //similar process as find pathname
    int counter = 0;
    int c;
    char *content_array = malloc(sizeof(char) * (content_length + 1));
    while (counter < content_length && (c = fgetc(stream)) != EOF) {
        *hash = blobby_hash(*hash, c);
        content_array[counter] = c;
        counter++;
    }
    content_array[counter] = '\0';
    char *content = content_array;
    return content;
}

void put_mode(mode_t st_mode, FILE *new_blob, int *hash) {
    //separate the mode into three bytes
    //and put those separated modes in the
    //new blob file
    mode_t mode;
    int counter = 0;
    while (counter < BLOBETTE_MODE_LENGTH_BYTES) {
        mode = st_mode << (8*counter);
        mode = mode >> 16;
        *hash = blobby_hash(*hash, mode);
        fputc(mode, new_blob);
        counter++;
    }
}

void put_pathname_length(FILE *new_blob, int pathname_length, int *hash) {
    //similar process as put mode
    uint16_t pathname_leng;
    int counter = 0;
    while (counter < BLOBETTE_PATHNAME_LENGTH_BYTES) {
        pathname_leng = pathname_length << (8*counter);
        pathname_leng = pathname_leng >> 8;
        *hash = blobby_hash(*hash, pathname_leng);
        fputc(pathname_leng, new_blob);
        counter++;
    }
}

void put_content_length(size_t st_size, FILE *new_blob, int *hash) {
    //similar process as put mode
    uint64_t content_length;
    int counter = 0;
    while (counter < BLOBETTE_CONTENT_LENGTH_BYTES) {
        content_length = st_size << (8*counter);
        content_length = content_length >> 40;
        *hash = blobby_hash(*hash, content_length);
        fputc(content_length, new_blob);
        counter++;
    }
}


// YOU SHOULD NOT CHANGE CODE BELOW HERE

// Lookup table for a simple Pearson hash
// https://en.wikipedia.org/wiki/Pearson_hashing
// This table contains an arbitrary permutation of integers 0..255

const uint8_t blobby_hash_table[256] = {
    241, 18,  181, 164, 92,  237, 100, 216, 183, 107, 2,   12,  43,  246, 90,
    143, 251, 49,  228, 134, 215, 20,  193, 172, 140, 227, 148, 118, 57,  72,
    119, 174, 78,  14,  97,  3,   208, 252, 11,  195, 31,  28,  121, 206, 149,
    23,  83,  154, 223, 109, 89,  10,  178, 243, 42,  194, 221, 131, 212, 94,
    205, 240, 161, 7,   62,  214, 222, 219, 1,   84,  95,  58,  103, 60,  33,
    111, 188, 218, 186, 166, 146, 189, 201, 155, 68,  145, 44,  163, 69,  196,
    115, 231, 61,  157, 165, 213, 139, 112, 173, 191, 142, 88,  106, 250, 8,
    127, 26,  126, 0,   96,  52,  182, 113, 38,  242, 48,  204, 160, 15,  54,
    158, 192, 81,  125, 245, 239, 101, 17,  136, 110, 24,  53,  132, 117, 102,
    153, 226, 4,   203, 199, 16,  249, 211, 167, 55,  255, 254, 116, 122, 13,
    236, 93,  144, 86,  59,  76,  150, 162, 207, 77,  176, 32,  124, 171, 29,
    45,  30,  67,  184, 51,  22,  105, 170, 253, 180, 187, 130, 156, 98,  159,
    220, 40,  133, 135, 114, 147, 75,  73,  210, 21,  129, 39,  138, 91,  41,
    235, 47,  185, 9,   82,  64,  87,  244, 50,  74,  233, 175, 247, 120, 6,
    169, 85,  66,  104, 80,  71,  230, 152, 225, 34,  248, 198, 63,  168, 179,
    141, 137, 5,   19,  79,  232, 128, 202, 46,  70,  37,  209, 217, 123, 27,
    177, 25,  56,  65,  229, 36,  197, 234, 108, 35,  151, 238, 200, 224, 99,
    190
};

// Given the current hash value and a byte
// blobby_hash returns the new hash value
//
// Call repeatedly to hash a sequence of bytes, e.g.:
// uint8_t hash = 0;
// hash = blobby_hash(hash, byte0);
// hash = blobby_hash(hash, byte1);
// hash = blobby_hash(hash, byte2);
// ...

uint8_t blobby_hash(uint8_t hash, uint8_t byte) {
    return blobby_hash_table[hash ^ byte];
}