/**
 * @file lcs.c
 * @author maszynatrurla
 * @brief Programming exercise: largest common subsequence
 * 
 * Inspiration: http://programmingpraxis.com/2009/06/09/longest-common-subsequence/
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define EOL_LEN 1


void printM(char * s0, char * s1, int * m, int l0, int l1) {
    int i, j;
    
    putchar(0x20);
    putchar(0x20);
    putchar(0x20);
    putchar(0x20);
    
    for (j = 0; j < l0 - 1; ++j) {
        putchar(s0[j]);
        putchar(0x20);
    }
    putchar('\n');
    
    for (j = 0; j < l1; ++j) {
        if (j) {
            putchar(s1[j - 1]);
        } else {
            putchar(0x20);
        }
        putchar(0x20);
        for (i = 0; i < l0; ++i) {
            printf("%d ", m[j * l0 + i]);
        }
        putchar('\n');
    }    
}

char * getLongestSubseq(char * s0, int l0, char * s1, int l1) {
    int i, j;
    int * m = calloc((++l0) * (++l1), sizeof(int));
    int ml = (l0 > l1 ? l0 : l1);
    char * res = calloc(ml, sizeof(char));
    int ci = ml - 1;
    
    if (!m || !res) {
        printf("Out of memory!\n");
        return 0;
    } 
    
    for (j = 0; j < l1; ++j) {
        for (i = 0; i < l0; ++i) {
            if (0 == i || j == 0) {
                m[j * l0 + i] = 0;
            } else if (s0[i - 1] == s1[j - 1]) {
                m[j * l0 + i] = m[(j - 1) * l0 + i - 1] + 1;
            } else {
                int o0 = m[j * l0 + i - 1];
                int o1 = m[(j - 1) * l0 + i];
                
                m[j * l0 + i] = o0 > o1 ? o0 : o1;
            }
        }
    }
    
    res[ci--] = 0;
    printM(s0, s1, m, l0, l1);
    
    for (j = l1 - 1; j > 0; --j) {
        for (i = l0 - 1; i > 0; --i) {
            if (s0[i - 1] == s1[j - 1]
                    && m[j * l0 + i] == m[(j - 1) * l0 + i - 1] + 1
                    && m[j * l0 + i] == m[(j - 1) * l0 + i] + 1
                    && m[j * l0 + i] == m[j * l0 + i - 1] + 1) {
                res[ci--] = s0[i - 1];
            }
        }
    }
    
    free(m);
    
    return &res[ci == ml - 1 ? ci : ci + 1];
}

int main(int argc, char ** argv) {
    char seq0 [BUFSIZ];
    char seq1 [BUFSIZ];
    int len0, len1;
    char * res;
    
    printf("Write two sequences delimited by end of line.\n");
    
    do {
        if (!fgets(seq0, sizeof(seq0), stdin)) {
            printf("You're no fun.\n");
            return 1;
        }
        len0 = strlen(seq0);
    } while (len0 <= EOL_LEN);
    
    len0 -= EOL_LEN;
    seq0[len0] = 0;
    
    do {
        if (!fgets(seq1, sizeof(seq1), stdin)) {
            printf("You're no fun.\n");
            return 1;
        }
        len1 = strlen(seq1);
    } while (len1 <= EOL_LEN);
    
    len1 -= EOL_LEN;
    seq1[len1] = 0;
    
    if (0 == (res = getLongestSubseq(seq0, len0, seq1, len1))) {
        return 1;
    }
    
    printf("\n%s\n", res);
    
    return 0;
}
