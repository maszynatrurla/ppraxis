
/**
 * @file sieve.c
 * @author maszynatrurla
 * @brief Programming exercise: sieve of Eratosthenes.
 * 
 * Inspiration: http://programmingpraxis.com/2009/02/19/sieve-of-eratosthenes/
 */

#include <stdio.h>
#include <math.h>

#define DEFAULT_N       15485863

int sieve(int N);

int sieve(int N) {
    int c = 0;
    int i;
    if (2 <= N) {
        printf("2\n");
        ++c;
    }
    for (i = 3; i <= N; i += 2) {
        int j;
        int pass = 1;
        for (j = 2; j <= sqrt(i); ++j) {
            if (0 == i % j) {
                pass = 0;
                break;
            }
        }
        if (pass) {
           // printf("%d\n", i);
            ++c; 
        }
    }
    return c;
}

int main(int argc, char ** argv) {
    int ret = sieve(DEFAULT_N);
    
    printf("\nFound %d primes\n", ret);
    return 0;
}
