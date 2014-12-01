#ifndef UTIL_H
#define UTIL_H

char *get_base64(char *in, char *out, int n);
char *fixed_xor(char *str_a, char *str_b, int n); 
char *hex2str(char *hex, char *str, int n);

#endif
