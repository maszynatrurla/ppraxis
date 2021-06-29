/**
 * @file flavius.c
 * @author maszynatrurla
 * @brief Programming exercise: cyclical elimination.
 * 
 * Inspiration: http://programmingpraxis.com/2009/02/19/flavius-josephus/
 */

#include <stdio.h>
#include <stdlib.h>

#define DEF_N   41
#define DEF_M    3

void printl(int * l, int len);

int * josephus(int n, int m) {
    int * killist = (int *) calloc(n, sizeof(int));
    int * reslist;
    int killidx = 0;
    int soldier = 0;
    
    if (!killist) {
        return 0;
    }
    
    while (killidx < n) {
        int count = m;
        while (1) {
            if (!killist[soldier % n]) {
                --count;
                if (0 == count) {
                    break;
                }
            }
            ++soldier;
        }
        killist[soldier % n] = ++killidx;
    }
    
    reslist = (int *) malloc(n * sizeof(int));
    if (!reslist) {
        return 0;
    }
    
    for (killidx = 0; killidx < n; ++killidx) {
        reslist[killist[killidx] - 1] = killidx;
    }
    
    free(killist);
    
    return reslist;
}

void printl(int * l, int len) {
    int i = 0;
    for (i = 0; i < len - 1; ++i) {
        if (0 == i % 20) {
            printf("\n");
        }
        printf("%d, ", l[i]);
    }
    if (i < len) {
        printf("%d\n", l[i]);
    } else {
        printf("\n");
    }
}

int main(int argc, char ** argv) {
    int * killed = josephus(DEF_N, DEF_M);
    
    if (killed) {
        printl(killed, DEF_N);
        free(killed);
    } else {
        printf("Out of memory.");
        return 1;
    }
    
    return 0;
}

