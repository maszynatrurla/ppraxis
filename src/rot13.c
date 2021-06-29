/**
 * @file rot13.c
 * @author maszynatrurla
 * @brief Programming exercise: Caesar cypher.
 * 
 * Inspiration: http://programmingpraxis.com/2009/02/20/rot13/
 */

#include <stdio.h>

#define ROT     13



int main(int argc, char ** argv) {
    while (1) {
        char c;
        
        c = getchar();
        if (EOF == c) {
            printf("\n");
            break;
        }
        if (c >= 'A' && c <= 'Z') {
            c -= 13;
            if (c < 'A') {
                c += 26;
            }
        } else if (c >= 'a' && c <= 'z') {
            c -= 13;
            if (c < 'a') {
                c += 26;
            }
        }
        printf("%c", c);
    }
    return 0;
}


