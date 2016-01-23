#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char base64_digits[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

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

        if ((k + 5) >= n) {
            fprintf(stderr, "get_base64(): provided buffer too small\n");
            return (out = NULL);
        }

        out[k++] = base64_digits[num >> 18]; /* leftmost 6 bit chunk */
        out[k++] = base64_digits[(num >> 12) & 0x3F]; /* center-left 6 bit chunk */
        out[k++] = (n_sig > 1) ? base64_digits[(num >> 6) & 0x3F] : '='; /* center-right 6 bit chunk */
        out[k++] = (n_sig > 2) ? base64_digits[num & 0x3F] : '='; /* rightmost 6 bit chunk */
    }

    out[k] = '\0';
    return out;
}

static int get_digit(char c)
{
    if ((c >= '0') && (c <= '9'))
        return (c - '0');
    else if ((c >= 'a') && (c <= 'f'))
        return (c - 'a') + 10;
    else if ((c >= 'A') && (c <= 'F'))
        return (c - 'A') + 10;
    else
        return -1;
}

static char get_ascii(int c)
{
    if ((c >= 0) && (c <= 9))
        return c + '0';
    else if ((c >= 10) && (c <= 15))
        return (c - 10) + 'a';
    else
        return -1;
}

char *fixed_xor(char *str_a, char *str_b, char *out, int n)
{
    int a, b, i;
    for (i = 0; i < n; i++) {
        a = get_digit(str_a[i]);
        b = get_digit(str_b[i]);
        out[i] = get_ascii(a ^ b);
    }

    out[i] = '\0';

    return out;
}

char *hex2str(char *hex, char *str, int n)
{
    int i, j = 0, stage = 0;
    char c;

    for (i = 0; hex[i] != '\0'; i++) {
       if (stage == 1) {
           c = (c * 16) + get_digit(hex[i]);
           stage = 0;

           if ((j + 1) >= n) {
               fprintf(stderr, "hex2str(): provided buffer too small\n");
               return (str = NULL);
           }

           str[j++] = c;
       } else {
           c = get_digit(hex[i]);
           stage = 1;
        }
    }

    str[j] = '\0';
    return str;
}

