/**
 * @file pancake.c
 * @author maszynatrurla
 * @brief Programming exercise: Pancake flipping sorting.
 * 
 * Inspiration: http://programmingpraxis.com/2009/04/07/flipping-pancakes/
 */

#include <stdio.h>

static void pancakeSort(int * tab, int len);
static void flip(int * tab, int len);
static void printt(int * tab, int len);

void pancakeSort(int * tab, int len) {
    int i;
    
    for (i = 0; i < len; ++i) {
        int j;
        int max = -1;
        int maxi = 0;
        
        for (j = 0; j < len - i; ++j) {
            if (max < tab[j]) {
                max = tab[j];
                maxi = j;
            }
        }
        if (maxi != len - i - 1) {
            flip(tab, maxi + 1);
            printt(tab, len);
            flip(tab, len - i);
        }
        printt(tab, len);
    }
}

void printt(int * tab, int len) {
    int i;
    
    for (i = 0; i < len; ++i) {
        printf("%d ", tab[i]);
    }
    putchar('\n');
}

void flip(int * tab, int len) {
    int i;
    
    for (i = 0; i < len / 2; ++i) {
        int tmp;
        
        tmp = tab[i];
        tab[i] = tab[len - 1 - i];
        tab[len - 1 - i] = tmp;
    }
}

int main(int argc, char ** argv) {
    int example0 [] = {6, 4, 7, 5, 2, 1, 8, 3, 0};
    int example1 [] = {42};
    int example2 [] = {12, 26, 30, 6, 25, 20, 18, 43, 19, 33, 5, 45, 40, 14,
            34, 15, 44, 38, 2, 9, 31, 21, 42, 17, 7, 8, 28, 46, 36, 47, 29,
            16, 22, 3, 39, 13, 24, 32, 4, 35, 23, 11, 27, 41, 10, 37};
    
    printt(example0, sizeof(example0) / sizeof(int));
    pancakeSort(example0, sizeof(example0) / sizeof(int));
    printt(example0, sizeof(example0) / sizeof(int));
    
    printt(example1, sizeof(example1) / sizeof(int));
    pancakeSort(example1, sizeof(example1) / sizeof(int));
    printt(example1, sizeof(example1) / sizeof(int));
    
    printt(example2, sizeof(example2) / sizeof(int));
    pancakeSort(example2, sizeof(example2) / sizeof(int));
    printt(example2, sizeof(example2) / sizeof(int));
    
    return 0;
}
