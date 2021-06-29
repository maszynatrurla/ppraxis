/**
 * @file chop.c
 * @author maszynatrurla
 * @brief Programming exercise: binary chop
 * 
 * Inspiration: http://programmingpraxis.com/2009/03/23/binary-search/
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define OK      0
#define NOK     1

#define TMAX    10734
#define TSTEP   6

#define ASSERT(X) if (!(X)) return NOK

int find(const int * tab, int len, int co);
int testSimp(void);
int testRand(void);
int check(const int * tab, int len, int co, int powinnoByc);

int check(const int * tab, int len, int co, int powinnoByc) {
    int wyszlo = find(tab, len, co);
    if (powinnoByc == wyszlo) {
        return OK;
    } else {
        int i;
        
        printf("FAILED while looking for %d in table:\n\t", co);
        for (i = 0; i < len; ++i) {
            printf("%d ", tab[i]);
        }
        printf("\nExpected index to be %d but got %d instead.\n",
               powinnoByc, wyszlo);
        return NOK;
    }
}

int find(const int * tab, int len, int co) {
    int off = 0;
    //printf ("Find called\n");
    while (len) {
        int nlen = len / 2;
        
        //printf("\ttab[%d + %d] = %d\n", off, nlen, tab[off + nlen]);
        
        if (tab[off + nlen] == co) {
            return off + nlen;
        } else if (tab[off + nlen] < co) {
            off += nlen;
            len = len - nlen;
        } else {
            len = nlen;
        }
    }
    
    return -1;
}

int testSimp() {
    int len = 0;
    int a[10] = {12, 13, 15, 23, 34, 35, 36, 45, 340, 987};
    
    ASSERT(OK == check(a, len, 0, -1));
    
    for (len = 1; len < sizeof(a) / sizeof(int); ++len) {
        int j;
        
        for (j = 0; j < len; ++j) {
            ASSERT(OK == check(a, len, a[j], j));
        }
    }
    
    printf("Passed: %s\n", __FUNCTION__);
    return OK;
}

int testRand() {
    int len = rand() % TMAX;
    int s = 0, i;
    int * tab = malloc(sizeof(int) * len);
    
    if (!tab) {
        printf("%s: Out of memory!\n", __FUNCTION__);
        return NOK;
    }
    
    for (i = 0; i < len; ++i) {
        s += 1 + (rand() % TSTEP);
        tab[i] = s;
    }
    
    for (i = 1; i < len; ++i) {
        int j;
        
        for (j = 0; j < i; ++j) {
            ASSERT(OK == check(tab, i, tab[j], j));
        }
    }
    printf("Passed: %s\n", __FUNCTION__);
    return OK;
}

int main(int argc, char ** argv) {
    srand(time(0));
    
    testSimp();
    testRand();
    return 0;
}
