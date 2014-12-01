#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char base64_digits[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char *get_base64(char *in, char *out, int n)
{
    int in_sz = strlen(in);
    int i = 0, j, k = 0, num = 0, seg, n_sig;
    char num_buf[7];

    while (i < in_sz) {
        n_sig = 3;

        for (j = 0; j < 6; j++) {
            if (i >= in_sz) {
                n_sig = i / 3;
                num_buf[j] = '0';
                continue;
            }

            num_buf[j] = in[i++];
        }

        num_buf[j] = '\0';
        num = strtol(num_buf, NULL, 16);
        printf("num_buf = %s, num = %d\n", num_buf, num);

        if ((k + 5) >= n) {
            fprintf(stderr, "get_base64(): out buffer too small\n");
            out = NULL;
            break;
        }

        /* 111111 = */
        out[k++] = base64_digits[num >> 18]; /* leftmost 6 bit chunk */
        out[k++] = base64_digits[(num >> 12) & 0x3F]; /* center-left 6 bit chunk */
        out[k++] = (n_sig > 1) ? base64_digits[(num >> 6) & 0x3F] : '='; /* center-right 6 bit chunk */ 
        out[k++] = (n_sig > 2) ? base64_digits[num & 0x3F] : '='; /* rightmost 6 bit chunk */
    }
    
    if (out)
        out[k] = '\0';

    return out;
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <string to convert>\n", argv[0]);
        exit(-1);
    }

    char base64[1024];
    
    printf("%s\n", get_base64(argv[1], base64, 1024));
    
    return 0;
}
