/**
 * @file morse.c
 * @author maszynatrurla
 * @brief Programming exercise: Morse code encoder/decoder.
 * 
 * Inspiration: http://programmingpraxis.com/2009/04/28/morse-code/
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define RANGE_A_START   'A'
#define RANGE_A_END     'Z'
#define RANGE_B_START   '0'
#define RANGE_B_END     '9'

typedef struct {
    unsigned char len;
    unsigned char code;
} MorseLetter;

static void encode(void);
static void decode(void);
static void printMorse(char letter);

const MorseLetter latinToMorseA [] = {
    {2, 0x1}, /* A */
    {4, 0xE}, /* B */
    {4, 0xA}, /* C */
    {3, 0x6}, /* D */
    {1, 0x1}, /* E */
    {4, 0xB}, /* F */
    {3, 0x4}, /* G */
    {4, 0xF}, /* H */
    {2, 0x3}, /* I */
    {4, 0x1}, /* J */
    {3, 0x2}, /* K */
    {4, 0xD}, /* L */
    {2, 0x0}, /* M */
    {2, 0x2}, /* N */
    {3, 0x0}, /* O */
    {4, 0x9}, /* P */
    {4, 0x4}, /* Q */
    {3, 0x5}, /* R */
    {3, 0x7}, /* S */
    {1, 0x0}, /* T */
    {3, 0x3}, /* U */
    {4, 0x7}, /* V */
    {3, 0x1}, /* W */
    {4, 0x6}, /* X */
    {4, 0x2}, /* Y */
    {4, 0xC}, /* Z */
};

const unsigned char latinToMorseB [] = {
    0, 1, 3, 7, 15, 31, 30, 28, 24, 16,
};

const char morseToLatin [] = {
  'E', 'T',
  'I', 'A', 'N', 'M',
  'S', 'U', 'R', 'W', 'D', 'K', 'G', 'O',
  'H', 'V', 'F', 0, 'L', 0, 'P', 'J', 'B', 'X', 'C', 'Y', 'Z', 'Q', 0, 0,
  
  '5', '4', 0, '3', 0, 0, 0, '2', 0, 0, 0, 0, 0, 0, 0, '1',
  '6', 0, 0, 0, 0, 0, 0, 0, '7', 0, 0, 0, '8', 0, '9', '0',
};

void printMorse(char letter) {
    if (letter >= RANGE_A_START && letter <= RANGE_A_END) {
        int i;
        const MorseLetter * pL = &latinToMorseA[(int) (letter - 'A')];
        
        for (i = 0; i < pL->len; ++i) {
            if (1 & (pL->code >> i)) {
                putchar('.');
            } else {
                putchar('-');
            }
        }
        putchar(' ');
    } else if (letter >= RANGE_B_START && letter <= RANGE_B_END) {
        int i;
        unsigned char code = latinToMorseB[(int) (letter - '0')];
        
        for (i = 0; i < 5; ++i) {
            if (1 & (code >> i)) {
                putchar('.');
            } else {
                putchar('-');
            }
        }
        putchar(' ');
    } else {
        putchar(letter);
    }
}

void encode() {
    char c;
    
    while (EOF != (c = getchar())) {
        printMorse(toupper(c));
    }
}

void decode() {
    char c;
    int power = 0;
    int i   = 0;
    int offset = 0;
    
    while (EOF != (c = getchar())) {
        if ((' ' == c || '\r' == c || '\n' == c) && power) {
            i = offset + i;
            if (i >= sizeof(morseToLatin)) {
                printf("\nWARN: incorrect Morse code found\n");
            } else {
                putchar(morseToLatin[i]);
            }
            power = 0;
            offset = 0;
            i = 0;
        } else if ('.' == c || '-' == c) {
            i = 2 * i + ('.' == c ? 0 : 1);
            offset += power;
            power = power == 0 ? 2 : power * 2;
        } else {
            putchar(c);
        }
    }    
}

int main(int argc, char ** argv) {
    if (argc != 2) {
        printf("Usage:\n%s encode|decode\n", argv[0]);
        return 1;
    }
    
    if (0 == strcmp("encode", argv[1])) {
        encode();
    } else if (0 == strcmp("decode", argv[1])) {
        decode();
    } else {
        printf("Unknown command: \"%s\"\n", argv[1]);
        return 1;
    }
    
    return 0;
}
