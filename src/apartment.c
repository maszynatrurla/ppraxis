/**
 * @file apartment.c
 * @author maszynatrurla
 * @brief Programming exercise: Find permutation that matches given criteria.
 * 
 * Inspiration: http://programmingpraxis.com/2009/02/20/multiple-dwellings/
 */

#include <stdio.h>
#include <stdlib.h>

#define TOP             5
#define BOT             1


inline int test(int b, int c, int f, int m, int s) {
    return b < TOP && c > BOT && f > BOT && f < TOP && m > c
    && s != f - 1 && s != f + 1
    && f != c - 1 && f != c + 1;
}

void swap(int * a, int i, int j) {
    int tmp = a[i];
    a[i] = a[j];
    a[j] = tmp;
}

void generate(int N, int * arr, int l) {
    if (1 == N) {
        if (test(arr[0], arr[1], arr[2], arr[3], arr[4])) {
            printf("Baker : %d\nCooper : %d\nFletcher : %d\n"
                            "Miller : %d\nSmith : %d\n",
                            arr[0], arr[1], arr[2], arr[3], arr[4]
                  );
        }
    } else {
        int i;
        for (i = 0; i < N - 1; ++i) {
            generate(N - 1, arr, l);
            if (N % 2) {
                swap(arr, 0, N - 1);
            } else {
                swap(arr, i, N - 1);
            }
        }
        generate(N - 1, arr, l);
    }
}


int main(int argc, char ** argv) {
    int A[] = {1, 2, 3, 4, 5};
    
    generate(5, A, 5);
    
    return 1;
}


