/**
 * @file automata.c
 * @author maszynatrurla
 * @brief Programming exercise: Elementary cellular automata.
 * 
 * Inspiration: http://programmingpraxis.com/2009/05/15/cellular-automata/
 */

#include <stdio.h>
#include <string.h>

#define ROWLEN  80

#define ONE     '#'
#define ZERO    ' '

static char row [2][ROWLEN];
static char * writeRow;
static char * readRow;

void initRow() {
    memset(row[0], ZERO, ROWLEN);
    row[0][ROWLEN / 2] = ONE;
    readRow = row[0];
    writeRow = row[1];
}

void printRow() {
    int i;
    for (i = 0; i < ROWLEN; ++i) {
        putchar(readRow[i]);
    }
    putchar('\n');
}

void usage(const char * name) {
    printf("Usage:\n%s RULE ROWS\n\nRULE = [0..255], ROWS = [0..]\n", name);
}

inline int calcState(char left, char middle, char right) {
    return (ONE == left ? 4 : 0) + (ONE == middle ? 2 : 0)
            + (ONE == right ? 1 : 0);
}

inline char nextState(state, rule) {
    return (1 & (rule >> state)) ? ONE : ZERO;
}

void iter(int rule) {
    int cellState;
    int i;
    /* edges first */
    cellState = calcState(readRow[ROWLEN - 1], readRow[0], readRow[1]);
    writeRow[0] = nextState(cellState, rule);
    
    cellState = calcState(readRow[ROWLEN - 2], readRow[ROWLEN - 1], readRow[0]);
    writeRow[ROWLEN - 1] = nextState(cellState, rule);
    
    for (i = 1; i < ROWLEN - 1; ++i) {
        cellState = calcState(readRow[i - 1], readRow[i], readRow[i + 1]);
        writeRow[i] = nextState(cellState, rule);
    }
}

void swapBufs() {
    if (readRow == row[0]) {
        readRow = row[1];
        writeRow = row[0];
    } else {
        readRow = row[0];
        writeRow = row[1];
    }
}

void automata(int rule, int rows) {
    int i;
    
    initRow();
    printRow();
    for (i = 0; i < rows; ++i) {
        iter(rule);
        swapBufs();
        printRow();
    }
}

int main(int argc, char ** argv) {
    int rule;
    int rows;
    if (argc != 3) {
        usage(argv[0]);
        return 1;
    }
    if (1 != sscanf(argv[1], "%d", &rule) || rule < 0 || rule > 255) {
        usage(argv[0]);
        return 1;
    }
    if (1 != sscanf(argv[2], "%d", &rows) || rows < 0) {
        usage(argv[0]);
        return 1;
    }
    
    automata(rule, rows);

    return 0;
}
