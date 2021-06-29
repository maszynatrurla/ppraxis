/**
 * @file sudoku.c
 * @author maszynatrurla
 * @brief Programming exercise: sudoku solver.
 * 
 * Inspiration: http://programmingpraxis.com/2009/02/19/sudoku/
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define DIM     9

typedef struct Crossroad_t {
    uint8_t opts[DIM];
    uint8_t index;
    uint8_t oi;
    struct Crossroad_t * prev;
} Crossroad;

static const char * parseArgs    (int argc,  char ** argv);
static int          readPuzzle   (FILE * fp, uint8_t (* table)[DIM]);
static int          solve        (uint8_t (* table)[DIM]);
static void         printPuzzle  (uint8_t (* table)[DIM]);
static int          emptySpace   (uint8_t (* table)[DIM]);
static Crossroad *  newDivergence(uint8_t index, uint8_t * opts,
                                  Crossroad * prev);
static int          getPossiblities(uint8_t index, uint8_t (* table)[DIM],
                                  uint8_t * possibilities);


static const char * parseArgs(int argc, char ** argv) {
    if (argc == 2) {
        return argv[1];
    }
    printf("Usage:\n%s filename\n", argv[0]);
    return 0;
}

static int readPuzzle(FILE * fp, uint8_t (* table)[DIM]) {
    char ch;
    int i = 0;
    uint8_t * flat = (uint8_t *) table;
    
    while (EOF != (ch = getc(fp))) {
        if (ch >= '0' && ch <= '9') {
            flat[i++] = ch - 0x30;
            if (i >= DIM * DIM) {
                return 1;
            }
        }
    }
    return 0;
}

static void printPuzzle(uint8_t (* table)[DIM]) {
    int i;
    
    for (i = 0; i < DIM; ++i) {
        int j;
        
        if (0 == i % 3) {
            printf("\n");
        }
        for (j = 0; j < DIM; ++j) {
            if (0 == j % 3) {
                printf(" ");
            }
            printf("%d ", table[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// void printpos(uint8_t * pos) {
//     int i = 0;
//     while (pos[i]) printf("%d,", pos[i++]);
// }

static int solve(uint8_t (* table)[DIM]) {
    int index;
    uint8_t * flat = (uint8_t *) table;
    Crossroad * crosses = 0;
    int iterc = 0;
    
    while (-1 != (index = emptySpace(table))) {
        uint8_t possibilities[DIM + 1] = {0,};
        
        ++iterc;
//         if (++iterc > 500000) return 0;
//         printf("\n%d. f[%d]=t[%d][%d] ", iterc, index, index / DIM, index % DIM);
        
        if (!getPossiblities(index, table, possibilities)) {
//             printf("noposs");
            while (1) {
                if (!crosses) {
                    return 0;
                }
                if (crosses->opts[crosses->oi]) {
                    break;
                } else {
                    Crossroad * tmp = crosses;
                    flat[crosses->index] = 0;
                    crosses = crosses->prev;
                    free(tmp);
                }
            }
            flat[crosses->index] = crosses->opts[crosses->oi++];
        } else {
//             printpos(possibilities);
            flat[index] = possibilities[0];
            if (crosses || possibilities[1]) {
                crosses = newDivergence(index, possibilities + 1, crosses);
                if (!crosses) {
                    printf("Out of memory\n");
                    return 0;
                }
            }
        }
        //printPuzzle(table);
    }
    
    printf("Finished after %d iterations\n", iterc);
    
    while (crosses) {
        Crossroad * tmp = crosses;
        crosses = crosses->prev;
        free(tmp);
    }
    
    return 1;
}

static Crossroad * newDivergence(uint8_t index, uint8_t * opts, Crossroad * prev) {
    Crossroad * cross = malloc(sizeof(Crossroad));
    if (cross) {
        cross->prev = prev;
        cross->index = index;
        cross->oi = 0;
        memcpy(cross->opts, opts, DIM);
    }
    return cross;
}

static int emptySpace(uint8_t (* table)[DIM]) {
    static int i = 0;
    uint8_t * flat = (uint8_t *) table;
    int j = 0;
    
    while(flat[i]) {
        ++i;
        ++j;
        if (i >= DIM * DIM) {
            i = 0;
        }
        if (j > DIM * DIM) {
            return -1;
        }
    }
    return i;
}

static int getPossiblities(uint8_t index, uint8_t (* table)[DIM],
                           uint8_t * possibilities) {
    uint8_t pre[DIM] = {0,};
    int x = index / DIM;
    int y = index % DIM;
    int ix = 3 * (x / 3);
    int iy = 3 * (y / 3);
    int i;
    int j;
    
//     printf("\n\t");
    for (i = 0; i < DIM; ++i) {
        if (table[x][i]) {
//             printf("%d,", table[x][i]);
            ++pre[table[x][i] - 1];
        }
    }
//     printf("\n\t");
    for (i = 0; i < DIM; ++i) {
        if (table[i][y]) {
//             printf("%d,", table[i][y]);
            ++pre[table[i][y] - 1];
        }
    }
//     printf("\n\t");
    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            if (table[ix + i][iy + j]) {
//                 printf("%d,", table[ix + i][iy + j]);
                ++pre[table[ix + i][iy + j] - 1];
            }
        }
    }
//     printf("\n\t\t");
    j = 0;
    for (i = 0; i < DIM; ++i) {
        if (!pre[i]) {
            possibilities[j++] = i + 1;
        }
    }
    possibilities[j] = 0;
    return j;
}


int main(int argc, char ** argv) {
    const char * filename;
    uint8_t puzzle[DIM][DIM];
    FILE * fp;
    
    filename = parseArgs(argc, argv);;
    if (!filename) {
        printf("Nothing to do.\n");
        return 1;
    }
    
    fp = fopen(filename, "r");
    if (!fp) {
        printf("Failed to open file.\n");
        return 1;
    }
    
    if (!readPuzzle(fp, puzzle)) {
        fclose(fp);
        printf("Failed to read puzzle from file.\n");
        return 1;
    }
    fclose(fp);
    
    printf("Puzzle to be solved:\n");
    printPuzzle(puzzle);
    
    if (!solve(puzzle)) {
        printf("Failed to solve puzzle.\n");
        return 1;
    }
    
    printf("Solved puzzle:\n");
    printPuzzle(puzzle);
    
    return 0;
}


