/**
 * @file primes.c
 * @author maszynatrurla
 * @brief Programming exercise: Primes puzzle
 * 
 * Inspiration: http://programmingpraxis.com/2009/04/14/google-treasure-hunt-2008-puzzle-4/
 */

#include <stdio.h>
#include <math.h>

/* #define DEBUGPRINT      1 */
#define DEBUGSTEP       100000

#define FINDC   1000000

#define OK      0
#define NOK     1

#define N0      541
#define N1      41
#define N2      17
#define N3      7

static int isPrime(int number);
static void sieve(void);
static int find(void);
static int initSum(int s);
static int nextSum(int prevSum, int s, int * idx);

static int primes[FINDC];

void sieve() {
    int i, j = 3;
#ifdef DEBUGPRINT
    int dbgi = DEBUGSTEP;
#endif
    
    primes[0] = 2;
    for (i = 1; i < FINDC;) {
#ifdef DEBUGPRINT
        if (i >= dbgi) {
            printf("%d\n", dbgi);
            dbgi += DEBUGSTEP;
        }
#endif
        if (OK == isPrime(j)) {
            primes[i++] = j;
        }
        j += 2;
    }
}

int isPrime(int number) {
    int i;
    
    for (i = 2; i < sqrt(number) + 1; ++i) {
        if (0 == number % i) {
            return NOK;
        }
    }
    return OK;
}

int initSum(int s) {
    int i;
    int sum = 0;
    
    for (i = 0; i < s; ++i) {
        sum += primes[i];
    }
    return sum;
}

int nextSum(int prevSum, int s, int * idx) {
    if (*idx + s < FINDC) {
        prevSum -= primes[*idx];
        prevSum += primes[*idx + s];
        *idx += 1;
        return prevSum;
    }
    return -1;
}

int find() {
    int sum0 = initSum(N0);
    int sum1 = initSum(N1);
    int sum2 = initSum(N2);
    int sum3 = initSum(N3);
    int idx0 = 0;
    int idx1 = 0;
    int idx2 = 0;
    int idx3 = 0;
    
    while (sum0 > 0) {
        if (OK == isPrime(sum0)) {
            while (sum1 < sum0) {
                sum1 = nextSum(sum1, N1, &idx1);
            }
            if (sum1 == sum0) {
                while (sum2 < sum0) {
                    sum2 = nextSum(sum2, N2, &idx2);
                }
                if (sum2 == sum0) {
                    while (sum3 < sum0) {
                        sum3 = nextSum(sum3, N3, &idx3);
                    }
                    if (sum3 == sum0) {
                        return sum0;
                    }
                }
            }
        }
        sum0 = nextSum(sum0, N0, &idx0);
#ifdef DEBUGPRINT
        if (0 == idx0 % 1000) {
            printf("%d(%d) %d(%d) %d(%d) %d(%d)\n",
                idx0, sum0, idx1, sum1, idx2, sum2,
                idx3, sum3);
        }
#endif
    }
    
    return 0;
}

int main(int argc, char ** argv) {
    int result;
    
    sieve();
    printf("%d primes found.\n", FINDC);
    
    result = find();
    
    printf("%d\n", result);
    
    return 0;
}
