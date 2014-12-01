#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

const char *freq_str = "EeTtAaOoIiNnSsHhRrDdLlUu";

struct byte_count {
    char byte;
    int count;
};

static void sort(struct byte_count *arr, int n)
{
    int i, j;
    struct byte_count key;

    for (i = 1; i < n; i++) {
        key = arr[i];
        for (j = (i - 1); (j >= 0) && (arr[j].count < key.count); j--)
            arr[j+1] = arr[j];
        arr[j+1] = key;
    }
}

/* basic frequency analysis, get the byte which appears most frequent and guess that it decodes to 'e' */
static char most_frequent_byte(char *str)
{
    struct byte_count bytes[255];
    int i, max = 0;

    for (i = 0; i < 255; i++) {
       bytes[i].byte  = i;
       bytes[i].count = 0;
    }

    for (i = 0; str[i] != '\0'; i++)
        bytes[str[i]].count++;

    sort(bytes, 255);
    
    return bytes[0].byte;
}

static int printable(char *str)
{
    int i;
    for (i = 0; str[i] != '\0'; i++) {
        if ((str[i] < 32) || (str[i] > 127))
            return 0;
    }
    
    return 1;
}

void decode(char *hex)
{
    char str[1024], key, byte;
    int i, j;

    hex2str(hex, str, 1024); 
    byte = most_frequent_byte(str);
    
    for (j = 0; freq_str[j] != '\0'; j++) { 
        key = byte ^ freq_str[j];
        for (i = 0; str[i] != '\0'; i++)
            str[i] ^= key;
        if (printable(str))
            printf("%c: %s\n", freq_str[j], str);
    }
}

int main(int argc, char **argv)
{
    FILE *strings_file; 
    char line_buffer[100], input;
    if (argc < 2)
        exit(0);

    if ((strings_file = fopen(argv[1], "r")) < 0) {
        perror("fopen");
        exit(-1);
    }

    while (fgets(line_buffer, 100, strings_file) != NULL) {
        printf("decoding %s\n", line_buffer);
        decode(line_buffer);
    }

    fclose(strings_file);

    exit(0);
}
