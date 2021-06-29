/**
 * @file wfactor.c
 * @author maszynatrurla
 * @brief Programming exercise: Wheel factorization.
 * 
 * Inspiration: http://programmingpraxis.com/2009/05/08/wheel-factorization/
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define N       600851475143UL

#define OK      0
#define NOK     1

void factorTrivial(unsigned long number);
void factorByWheel(unsigned * w, int len, unsigned long number);

int  initWheel(unsigned * w, int len);
void disposeWheel();

static unsigned * wheel = 0;
static unsigned wheelLen = 0;

unsigned int nextPrime(unsigned int prime) {
    for (prime = prime + 2;; prime += 2) {
        int i;
        int until = sqrt(prime);
        for (i = 3; i <= until; ++i) {
            if (0 == prime % i) {
                i = 0;
                break;
            }
        }
        if (i) {
            return prime;
        }
    }
    return 0;
}

int initWheel(unsigned * w, int len) {
    unsigned c = 1;
    unsigned t;
    int i;
    
    for (i = 0; i < len; ++i) {
        c *= w[i];
    }
    
    wheelLen = 0;
    wheel = malloc(sizeof(unsigned) * c);
    if (!wheel) {
        return NOK;
    }
    
    for (i = 1; i < c; ++i) {
        int j;
        for (j = 0; j < len; ++j) {
            if (0 == i % w[j]) {
                break;
            }
        }
        if (j == len) {
            wheel[wheelLen++] = i;
//             printf("%d ", i);
        }
    }
    
    t = wheel[0];
    wheel[0] = (c - wheel[wheelLen - 1]) + wheel[0];
    for (i = 1; i < wheelLen; ++i) {
        unsigned pt = t;
        t = wheel[i];
        wheel[i] = wheel[i] - pt;
    }
//     putchar('\n');
//     for (i = 0; i < wheelLen; ++i) {
//         printf("%d ", wheel[i]);
//     }
    
    return OK;
}

void disposeWheel() {
    if (wheel) {
        free(wheel);
    }
}

void factorTrivial(unsigned long number) {
    unsigned long div;
    unsigned long num = number;
    unsigned long until = sqrt(num);
    int fc = 0;
    
    for (div = 2; div <= until; ++div) {
        if (0 == num % div) {
            ++fc;
            printf("%ld\n", div);
            num /= div;
        }
    }
    if (fc) {
        printf("Number %ld has %d factors\n", number, fc);
    }
    
}

void factorByWheel(unsigned * w, int len, unsigned long number) {
    unsigned long div;
    int i;
    unsigned long num = number;
    unsigned long until = sqrt(number);
    int fc = 0;
    
    for (i = 0; i < len; ++i) {
        if (0 == num % w[i]) {
            ++fc;
            printf("%d\n", w[i]);
            num /= w[i];
        }
    }
    div = nextPrime(w[len - 1]);
    if (0 == num % div) {
        ++fc;
        printf("%ld\n", div);
        num /= div;
    }
    i = 0;
    while (div <= until) {
        div += wheel[i++];
        if (0 == num % div) {
            ++fc;
            printf("%ld\n", div);
            num /= div;
        }
        
        if (i >= wheelLen) {
            i = 0;
        }
    }
    if (num > 1) {
        ++fc;
        printf("%ld\n", num);
    }
    
    if (fc) {
        printf("Number %ld has %d factors\n", number, fc);
    }
}



int main(int argc, char ** argv) {
    unsigned w [] = {2, 3, 5, 7};
    
    factorTrivial(N);
    
    initWheel(w, sizeof(w) / sizeof(unsigned));
    factorByWheel(w, sizeof(w) / sizeof(unsigned), N);
    disposeWheel();
    
    return 0;
}
