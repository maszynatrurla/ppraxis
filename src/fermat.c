/**
 * @file fermat.c
 * @author maszynatrurla
 * @brief Programming exercise: Fermat's factorization
 * 
 * Inspiration: http://programmingpraxis.com/2009/05/19/fermats-method/
 */

#include <stdio.h>
#include <math.h>
#include <time.h>

#define N       5959


void fopt(int n, int * xy) {
    int x = (int) (sqrt((double) n) + 1.);
    int y = 0;
    int r = x * x - n;
    int u = x + x + 1;
    int v = 1;
    
    while (r != 0) {
        if (x == n) {
            *xy = x;
            *(xy + 1) = 1;
            return;
        }
        if (r < 0) {
            ++x;
            r += u;
            u += 2;
        } else {
            ++y;
            r -= v;
            v += 2;
        }
    }
    *xy = x - y;
    *(xy + 1) = x + y;
}

void factor(int n) {
    int xy [2];
    fopt(n, xy);
    if (1 == xy[0]) {
        printf("%d ", xy[1]);
    } else if (1 == xy[1]) {
        printf("%d ", xy[0]);
    } else {
        factor(xy[0]);
        factor(xy[1]);
    }
}


int main(int argc, char ** argv) {
    int input;
    
    if (2 != argc) {
        printf("Usage:\n%s NUMBER\n", argv[0]);
        return 1;
    }
    
    if (1 != sscanf(argv[1], "%d", &input) || input < 2) {
        printf("Argument should be a positive integer larger than one.\n");
        return 1;
    }
    
    factor(input);
    putchar('\n');

    return 0;
}
