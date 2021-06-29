/**
 * @file wsearch.c
 * @author maszynatrurla
 * @brief Programming exercise: Word search solver
 * 
 * Inspiration: http://programmingpraxis.com/2009/05/26/word-search-solver/
 */

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define OK      0
#define NOK     1

#define PAGE    100

typedef enum {
    NotFound    = 0,
    Forward,
    Backward,
    Up,
    Down,
    UpForward,
    UpBackward,
    DownForward,
    DownBackward,
} WordPosition;

static int initPuzzle(char ** words, int wordCount);
static int appendC(char c);
static const char * postostr(WordPosition pos);

static struct {
    int capacity;
    int length;
    char * grid;
    int rowlen;
    int rowcount;
    char ** words;
    int wordCount;
} puzzle;

static const char * postostr(WordPosition pos) {
    switch (pos) {        
        case Forward     :
            return "Forward";
        case Backward    :
            return "Backward";
        case Up          :
            return "Up";
        case Down        :
            return "Down";
        case UpForward   :
            return "Up Forward";
        case UpBackward  :
            return "Up Backward";
        case DownForward :
            return "Down Forward";
        case DownBackward:
            return "Down Backward";
        case NotFound    :
        default:
            return "Not Found";
    }
}

static int initPuzzle(char ** words, int wordCount) {
    puzzle.words = words;
    puzzle.wordCount = wordCount;
    puzzle.length = 0;
    puzzle.rowlen = 0;
    puzzle.rowcount = 0;
    puzzle.grid = malloc(PAGE);
    if (!puzzle.grid) {
        printf("Out of memory\n");
        return NOK;
    }
    puzzle.capacity = PAGE;
    return OK;
}

int appendC(char c) {
    static int previousRC = 0;
    if (puzzle.length >= puzzle.capacity) {
        puzzle.grid = realloc(puzzle.grid, puzzle.capacity + PAGE);
        if (!puzzle.grid) {
            printf("Out of memory\n");
            return NOK;
        }
        puzzle.capacity += PAGE;
    }
    if (puzzle.rowcount > previousRC) {
        if (0 == puzzle.rowlen) {
            puzzle.rowlen = puzzle.length;
        } else if (0 != puzzle.length % puzzle.rowlen) {
            printf("Rows are not equal in length\n");
            return NOK;
        }
        previousRC = puzzle.rowcount;
    }
    puzzle.grid[puzzle.length++] = c;
   
    return OK;
}


int readGrid(const char * file) {
    FILE * fp;
    char c;
    int eol = 0;
    
    fp = fopen(file, "r");
    if (!fp) {
        printf("Cannot read puzzle file: \"%s\"\n", file);
        return NOK;
    }
    
    while (EOF != (c = fgetc(fp))) {
        if (eol) {
            switch (c) {
                case 0xA:
                case 0xD:
                case 0x20:
                case 0x9:
                    continue;
                default:
                    if (OK != appendC(toupper(c))) {
                        fclose(fp);
                        return NOK;
                    }
                    eol = 0;
                    break;
            }
        } else {
            switch (c) {
                case 0x20:
                case 0x9:
                    continue;
                case 0xA:
                case 0xD:
                    if (puzzle.length) {
                        eol = 1;
                        ++puzzle.rowcount;
                    }
                    break;
                default:
                    if (OK != appendC(toupper(c))) {
                        fclose(fp);
                        return NOK;
                    }
                    break;
            }
        }
    }
    if (!eol) {
        if (0 != puzzle.length % puzzle.rowlen) {
            printf("Rows are not equal in length\n");
            fclose(fp);
            return NOK;
        }
        ++puzzle.rowcount;
    }
    
    fclose(fp);
    return OK; 
}

void printPuz() {
    int i, j = 0;
    
    printf("Trying to find words: ");
    for (i = 0; i < puzzle.wordCount; ++i) {
        printf("%s, ", puzzle.words[i]);
    }
    printf("\nin letter grid:\n\t    ");
    for (i = 0; i < puzzle.rowlen; ++i) {
        printf("%d ", i + 1);
    }
    for (i = 0; i < puzzle.length; ++i) {
        if (0 == i % puzzle.rowlen) {
            ++j;
            printf("\n\t%3d ", j);
            
        }
        putchar(puzzle.grid[i]);
        putchar(0x20);
    }
    putchar('\n');
}

int findForward(const char * word, int idx) {
    int j;
    int until;
    
    until = puzzle.rowlen - (idx % puzzle.rowlen);
    for (j = 0; word[j] && j < until; ++j) {
        if (word[j] != puzzle.grid[idx + j]) {
            return NOK;
        }
    }
    return word[j] ? NOK : OK;
}

int findBackward(const char * word, int idx) {
    int j;
    int until;
    
    until = (idx % puzzle.rowlen) + 1;
    for (j = 0; word[j] && j < until; ++j) {
        if (word[j] != puzzle.grid[idx - j]) {
            return NOK;
        }
    }
    return word[j] ? NOK : OK;
}

int findUp(const char * word, int idx) {
    int j;
    int until;
    
    until = (idx / puzzle.rowlen) + 1;
    for (j = 0; word[j] && j < until; ++j) {
        if (word[j] != puzzle.grid[idx - j * puzzle.rowlen]) {
            return NOK;
        }
    }
    return word[j] ? NOK : OK;
}

int findDown(const char * word, int idx) {
    int j;
    int until;
    
    until = puzzle.rowcount - (idx / puzzle.rowlen);
    for (j = 0; word[j] && j < until; ++j) {
        if (word[j] != puzzle.grid[idx + j * puzzle.rowlen]) {
            return NOK;
        }
    }
    return word[j] ? NOK : OK;
}

int findUpFwd(const char * word, int idx) {
    int j;
    int untilx = puzzle.rowlen - (idx % puzzle.rowlen);
    int untily = (idx / puzzle.rowlen) + 1;
    int until = untilx > untily ? untily : untilx;
    
    for (j = 0; word[j] && j < until; ++j) {
        if (word[j] != puzzle.grid[idx - j * puzzle.rowlen + j]) {
            return NOK;
        }
    }
    return word[j] ? NOK : OK;
}

int findUpBck(const char * word, int idx) {
    int j;
    int untilx = (idx % puzzle.rowlen) + 1;
    int untily = (idx / puzzle.rowlen) + 1;
    int until = untilx > untily ? untily : untilx;
    
    for (j = 0; word[j] && j < until; ++j) {
        if (word[j] != puzzle.grid[idx - j * puzzle.rowlen - j]) {
            return NOK;
        }
    }
    return word[j] ? NOK : OK;
}

int findDownFwd(const char * word, int idx) {
    int j;
    int untilx = puzzle.rowlen - (idx % puzzle.rowlen);
    int untily = puzzle.rowlen - (idx / puzzle.rowlen);
    int until = untilx > untily ? untily : untilx;
    
    for (j = 0; word[j] && j < until; ++j) {
        if (word[j] != puzzle.grid[idx + j * puzzle.rowlen + j]) {
            return NOK;
        }
    }
    return word[j] ? NOK : OK;
}

int findDownBack(const char * word, int idx) {
    int j;
    int untilx = (idx % puzzle.rowlen) - 1;
    int untily = puzzle.rowlen - (idx / puzzle.rowlen);
    int until = untilx > untily ? untily : untilx;
    
    for (j = 0; word[j] && j < until; ++j) {
        if (word[j] != puzzle.grid[idx + j * puzzle.rowlen - j]) {
            return NOK;
        }
    }
    return word[j] ? NOK : OK;
}

WordPosition findWordAt(const char * word, int idx) {
    if (OK == findForward(word, idx)) {
        return Forward;
    }
    if (OK == findBackward(word, idx)) {
        return Backward;
    }
    if (OK == findUp(word, idx)) {
        return Up;
    }
    if (OK == findDown(word, idx)) {
        return Down;
    }
    if (OK == findUpFwd(word, idx)) {
        return UpForward;
    }
    if (OK == findUpBck(word, idx)) {
        return UpBackward;
    }
    if (OK == findDownFwd(word, idx)) {
        return DownForward;
    }
    if (OK == findDownBack(word, idx)) {
        return DownBackward;
    }

    return NotFound;
}

void findWord(const char * word) {
    int i;
    
    for (i = 0; i < puzzle.length; ++i) {
        WordPosition pos;
        
        if (puzzle.grid[i] == word[0]) {
            if (NotFound != (pos = findWordAt(word, i))) {
                printf("Word %-10s: Row %d Column %d %s\n",
                        word, i / puzzle.rowlen + 1, i % puzzle.rowlen + 1,
                        postostr(pos));
                return;
            }
        }
    }
    printf("Word %-10s: not found\n", word);
}

int main(int argc, char ** argv) {
    int i;
    
    if (argc < 3) {
        printf("Usage:\n%s FILE WORD1 [WORD2 [...]]\n", argv[0]);
        return 1;
    }
    if (OK != initPuzzle(&argv[2], argc - 2)) {
        return 1;
    }
    if (OK != readGrid(argv[1])) {
        return 1;
    }
    
    printPuz();
    putchar('\n');
    
    for (i = 0; i < puzzle.wordCount; ++i) {
        findWord(puzzle.words[i]);
    }

    return 0;
}
