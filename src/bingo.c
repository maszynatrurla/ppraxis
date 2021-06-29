/**
 * @file bingo.c
 * @author maszynatrurla
 * @brief Programming exercise: bingo probability apporximation.
 * 
 * Inspiration: http://programmingpraxis.com/2009/02/19/bingo/
 */
 
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define NO_TRIES_1        100000
#define NO_TRIES_2        1000

/**
 * Structure of bingo player's card.
 * Stores data about drawn numbers and correct calls.
 */
typedef struct Card_t {
    /**
     * Table for storing drawn numbers and their position on the card.
     * Cell index represents drawn number and cell value position on
     * the card. Value equal to zero means that the number was not drawn.
     * Example: if drawn[3] = 5, fifth column in first row on the card
     * has number 3 on it.
     * This means using a bit excessive amout of memory, but it makes
     * calling and checking faster.
     */
    uint8_t drawn[75];
    /**
     * Number of correctly called numbers on the card rows.
     */
    uint8_t occupiedX[5];
    /**
     * Number of correctly called numbers on the card columns.
     */
    uint8_t occupiedY[5];
    /**
     * Number of correctly called numbers on the diagonal.
     */
    uint8_t diagA;
    /**
     * Number of correctly called numbers on the other diagonal.
     */
    uint8_t diagB;
} Card;

static void    prepare (Card * card);
static int     solve   (Card * card);
static uint8_t draw    (uint8_t * space, int length);
static int     call    (Card * card, uint8_t called);
static int     solveAll(Card * cards, int length);

static void prepare(Card * card) {
    int i;
    
    /* zero the card and results */
    memset(card, 0, sizeof(struct Card_t));
    
    /* fill the card, iterate by position
       on the card (there are 25 cells, divided by
       5 columns, one cell is pre-occupied).
       */
    for (i = 0; i < 25; ++i) {
        uint8_t draws[15];
        
        if (12 == i) {
            /* Center space (third row, third column) is pre-occupied */
            continue;
        }
        if (0 == (i % 5)) {
            memset(draws, 0, 15);
        }
        /* First  row i = [ 0-4 ]: random numbers from  0 to 14
           Second row i = [ 5-9 ]: random numbers from 15 to 29
           Third  row i = [10-14]: random numbers from 30 to 44
           Fourth row i = [15-19]: random numbers from 45 to 59
           Fifth  row i = [20-24]: random numbers from 60 to 74 */
        card->drawn[draw(draws, 15) + (15 * (i / 5))] = i + 1;
    }
    /* count pre-occupied place as called */
    card->occupiedX[2] = 1;
    card->occupiedY[2] = 1;
    card->diagA = 1;
    card->diagB = 1;
}

static int call(Card * card, uint8_t called) {
    uint8_t matchPosition;
    
    if (0 != (matchPosition = card->drawn[called])) {
        uint8_t x = (matchPosition - 1) / 5;
        uint8_t y = (matchPosition - 1) % 5;
        
        if (++card->occupiedX[x] >= 5) {
            return 1;
        }
        if (++card->occupiedY[y] >= 5) {
            return 1;
        }
        
        --matchPosition;
        
        if ((4 == matchPosition || 8 == matchPosition
                || 12 == matchPosition || 16 == matchPosition
                || 20 == matchPosition) && ++card->diagA >=5) {
            return 1;
        } else if ((0 == matchPosition || 6 == matchPosition
                || 12 == matchPosition || 18 == matchPosition
                || 24 == matchPosition) && ++card->diagB >= 5) {
            return 1;
        }
    }
    return 0;
}

static int solve(Card * card) {
    int calls = 0;  /* calls counter */
    uint8_t space[75] = {0,};
    
    while (1) {
        ++calls;
        if (call(card, draw(space, 75))) {
            return calls;
        }
    }
}

static int solveAll(Card * cards, int length) {
    int i;
    int calls = 0;
    uint8_t space[75] = {0,};
    
    for (i = 0; i < length; ++i) {
        prepare(&cards[i]);
    }
    while (1) {
        uint8_t called = draw(space, 75);
        ++calls;
        for (i = 0; i < length; ++i) {
            if (call(&cards[i], called)) {
                return calls;
            }
        }
    }
    
}

static uint8_t draw(uint8_t * space, int length) {
    uint8_t drawn;
    do {
        drawn = rand() % length;
    } while (space[drawn]);
    ++space[drawn];
    return drawn;
}

int main(int argc, char ** argv) {
    int i;
    uint32_t calls = 0;
    
    srand(time(0));
    {
        for (i = 0; i < NO_TRIES_1; ++i) {
            Card card;
            
            prepare(&card);
            calls += solve(&card);
        }
        printf("1. %d\n", calls / NO_TRIES_1);
    }
    {
        Card cards[500];
        calls = 0;
        
        for (i = 0; i < NO_TRIES_2; ++i) {
            calls += solveAll(cards, 500);
        }
        printf("2. %d\n", calls / NO_TRIES_2);
    }
    
    
    return 0;
}
 