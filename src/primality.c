/**
 * @file primality.c
 * @author maszynatrurla
 * @brief Programming exercise: Check if number is a prime by Miller-Rabin method.
 * 
 * Inspiration: http://programmingpraxis.com/2009/05/01/primality-checking/
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* 2**89 - 1 */
#define NUMBER_HI         0x1ffffffUL
#define NUMBER_LO         0xffffffffffffffffUL
#define K                 50

#define OK      0
#define NOK     1

typedef struct {
    unsigned long long hi;
    unsigned long long lo;
} BN;


static BN  mi  (BN a, BN b);
static BN  cBN (unsigned u);

static int find_r_s(BN a, unsigned * r, BN * s);
static BN  find_a(BN n);
static int check(BN n, BN a, BN s, unsigned r);

static void printbn(const char * pref, const BN * a);


BN mi(BN a, BN b) {
    BN r;
    r.hi = a.hi - b.hi;
    r.lo = a.lo - b.lo;
    if (r.lo > a.lo) {
        --r.hi;
    }
    return r;
}


BN cBN(unsigned u) {
    BN r;
    r.hi = 0;
    r.lo = u;
    return r;
}

int find_r_s(BN a, unsigned * r, BN * s) {
    __int128 A = (((__int128) a.hi) << 64) | ((__int128) a.lo);
    A -= 1;
    
    for (*r = 0; *r < 128; ++(*r)) {
        __int128 B = ((__int128) 1) << *r;
        __int128 S = A / B;
        
        if (0 != A % B) {
            continue;
        }
        if (1 == (S % 2)) {
            s->hi = (long long) (S >> 64);
            s->lo = (long long) S;
            return OK;
        }
    }
    return NOK;
}

void printbn(const char * pref, const BN * a) {
    printf("%s%llX%08llX\n", pref, a->hi, a->lo);
}

BN find_a(BN n) {
    BN a;
    BN N = mi(n, cBN(2));
    a.lo = 1 + (rand() % N.lo);
    if (N.hi) {
        a.hi = rand() % N.hi;
    }
    return a;
}


inline __int128 powmod(__int128 b, __int128 e, __int128 m) {
    __int128 r = 1;
    b = b % m;
    while (e > 0) {
        if (1 == e % 2) {
            r = (r * b) % m;
        }
        e >>= 1;
        b = (b * b) % m;
    }
    return r;
}



int check(BN n, BN a, BN s, unsigned r) {
    unsigned j;
    __int128 A = (((__int128) a.hi) << 64) | ((__int128) a.lo);
    __int128 S = (((__int128) s.hi) << 64) | ((__int128) s.lo);
    __int128 N = (((__int128) n.hi) << 64) | ((__int128) n.lo);
    /* a**s % n === 1 (mod n) */
    if ((__int128)1 == powmod(A, S, N)) {
        return 1;
    }
    /* a**(2**j * s) == -1 (mod n) */
    for (j = 0; j < r; ++j) {
        __int128 pj = ((__int128) 1) << j;
        pj *= S;
        if (N - 1 == powmod(A, pj, N)) {
            return 1;
        }
    }
    return 0;
}

int main(int argc, char ** argv) {
    BN n = {NUMBER_HI, NUMBER_LO};
    BN s;
    BN a;
    unsigned r;
    int i;
    
    srand(time(0));
    
    if (OK != find_r_s(n, &r, &s)) {
        printf("Something went wrong\n");
        return 1;
    }
    printbn("s = ", &s);
    printf("r = %d\n", r);
    
    for (i = 0; i < K; ++i) {
        a = find_a(n);
        printf("%d. ", i);
        printbn("a = ", &a);
        if (!check(n, a, s, r)) {
            printf("Not a prime.\n");
            return 0;
        }
    }
    
    printf("Prime!\n");
    
    return 0;
}
