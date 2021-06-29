/**
 * @file palindrome.c
 * @author maszynatrurla
 * @brief Programming exercise: Find next palindromic number.
 * 
 * Inspiration: http://programmingpraxis.com/2009/05/22/the-next-palindrome/
 */

#include <stdio.h>

#define MAXDIGITS       1000

#define OK      0
#define NOK     1

static char * checkArg(char * num, char ** outEnd);

static char * plus1(char * begin, char * end, int * newLen);

static char buf [MAXDIGITS + 2];

static char * checkArg(char * num, char ** outEnd) {
    int i = 0;
    int off = 1;
    int leading = 1;
    
    for (i = 0; num[i]; ++i) {
        if (i > MAXDIGITS || num[i] < '0' || num[i] > '9') {
            return 0;
        }
        if (leading && '0' == num[i]) {
            continue;
        } else {
            leading = 0;
            buf[off++] = num[i];
        }
    }
    if (1 == off) {
        buf[off++] = '0';
    }
    buf[off] = 0;
    *outEnd = &buf[off];
    return buf + 1;
}

static char * plus1(char * begin, char * end, int * newLen) {
    while (end > begin) {
        end -= 1;
        if (*end < '9') {
            ++(*end);
            return begin;
        } else {
            *end = '0';
        }
    }
    buf[0] = '1';
    *newLen += 1;
    return buf;
}

void palindrome(char * a, char * end) {
    int numlen = end - a;
    int until;
    int p;
    int i;
    
    a = plus1(a, end, &numlen);
    until = (numlen + 1) / 2;
    p = (0 == numlen % 2) ? 1 : 0;
    numlen /= 2;
    
    for (i = 0; i < until; ++i) {
        char * hi = &a[numlen - i - p];
        char * lo = &a[numlen + i];
        
        if (*hi == *lo) {
            continue;
        }
        if (*hi > *lo) {
            *lo = *hi;
            continue;
        }
        {
            int dummy;
            *lo = '0';
            plus1(a, lo, &dummy);
            /* restart loop */
            i = -1;
        }
        
    }
    
    printf("%s\n", a);
}


int main(int argc, char ** argv) {
    char * input;
    char * end;
    
    if (argc != 2) {
        printf("Usage:\n%s NUMBER\n", argv[0]);
        return 1;
    }
    if (0 == (input = checkArg(argv[1], &end))) {
        printf("Incorrect argument, should be a positive decimal number with"
                " no more than %d digits.\n", MAXDIGITS);
        return 1;
    }
    
    palindrome(input, end);

    return 0;
}
