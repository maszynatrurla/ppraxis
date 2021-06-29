/**
 * @file roman.c
 * @author maszynatrurla
 * @brief Programming exercise: Roman numerals.
 * 
 * Inspiration: http://programmingpraxis.com/2009/03/06/roman-numerals/
 */

#include <stdio.h>

static int  digit   (char c);
static int  toDec   (const char * roman);
static void writeRom(int decimal);

void add_roman(const char * A, const char * B) {
    writeRom(toDec(A) + toDec(B));
}

void writeRom(int decimal) {
    int i;
    int res;

    res = decimal / 1000;
    for (i = 0; i < res; ++i) putchar('M');
    decimal = decimal % 1000;
    
    if (900 <= decimal) {
        putchar('C');
        putchar('M');
        decimal -= 900;
    }
    
    if (500 <= decimal) {
        putchar('D');
        decimal -= 500;
    }
    
    if (400 <= decimal) {
        putchar('C');
        putchar('D');
        decimal -= 400;
    }
    
    res = decimal / 100;
    for (i = 0; i < res; ++i) putchar('C');
    decimal = decimal % 100;
    
    if (90 <= decimal) {
        putchar('X');
        putchar('C');
        decimal -= 90;
    }
    
    if (50 <= decimal) {
        putchar('L');
        decimal -= 50;
    }
    
    if (40 <= decimal) {
        putchar('X');
        putchar('L');
        decimal -= 40;
    }
    
    res = decimal / 10;
    for (i = 0; i < res; ++i) putchar('X');
    decimal = decimal % 10;
    
    if (5 <= decimal) {
        putchar('V');
        decimal -= 5;
    }
    
    if (4 <= decimal) {
        putchar('I');
        putchar('V');
        decimal -= 4;
    }
    
    for (i = 0; i < decimal; ++i) putchar('I');
    
    putchar('\n');
}

int toDec(const char * roman) {
    int sum = 0;
    int prev = 0;
    int i;
    
    for (i = 0; roman[i]; ++i) {
        int next = digit(roman[i]);
        sum += next;
        if (prev && next > prev) {
            sum -= 2 * prev;
        }
        prev = next;
    }
    
    return sum;
}

int digit(char c) {
    switch (c) {
        case 'M':
        case 'm':
            return 1000;
        case 'D':
        case 'd':
            return 500;
        case 'C':
        case 'c':
            return 100;
        case 'L':
        case 'l':
            return 50;
        case 'X':
        case 'x':
            return 10;
        case 'V':
        case 'v':
            return 5;
        case 'I':
        case 'i':
            return 1;
        default:
            return 0;
    }
}

int main(int argc, char ** argv) {
    if (2 == argc) {
        int a;
        //printf("%d\n", toDec(argv[1]));
        sscanf(argv[1], "%d", &a);
        writeRom(a);
    }
    
    add_roman("CCCLXIX", "CDXLVIII");

    return 0;
}



