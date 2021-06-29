/**
 * @file pii.c
 * @author maszynatrurla
 * @brief Programming exercise: Spigot algorithm for PI.
 * 
 * Inspiration: http://programmingpraxis.com/2009/02/20/the-digits-of-pi/
 */

#include <stdio.h>

#define DIGIT   1001

void init(int * t, int l) {
    int i;
    for (i = 0; i < l; ++i) {
        t[i] = 2;
    }
}


int main(int argc, char ** argv) {
    int t[(int) (DIGIT * 3.32)];
    int i;
    int l = sizeof(t) / sizeof(int);
    int pidigit, poprzednia = -1;
    
    init(t, l);
    
    for (i = 0; i < DIGIT; ++i) {
        int j;
        int carry = 0;
        
        for (j = 0; j < l; ++j) {
            int A = l - 1 - j;
            int B = 2 * A + 1;
            
            if (l - 1 == j) {
                t[j] *= 10;
                t[j] += carry;
                pidigit = t[j] / 10;
                if (pidigit > 9) {
                    pidigit = pidigit % 10;
                    poprzednia += 1;
                }
                if (poprzednia >= 0) {
                    printf("%d", poprzednia);
                }
                poprzednia = pidigit;
                t[j] = t[j] % 10;
            } else {
                t[j] *= 10;
                t[j] += carry;
                carry = (t[j] / B) * A;
                t[j] = t[j] % B;
            }
        }
        carry = 0;
    }
    
    printf("%d\n", pidigit);
    
    return 0;
}

