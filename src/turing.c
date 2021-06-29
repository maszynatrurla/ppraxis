/**
 * @file turing.c
 * @author maszynatrurla
 * @brief Programming exercise: Turing Machine simulator
 *
 * Inspiration: http://programmingpraxis.com/2009/03/27/a-turing-machine-simulator/
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define STEP_BY_STEP_DEBUG

#define OK              0
#define NOK             1

#define MAXMEM          1000000
#define MEMUNIT         1000

#define BLANK           '_'
#define PTR_L           '['
#define PRT_R           ']'
#define RIGHT           'R'
#define LEFT            'L'

#define BLANKREP        5

typedef struct {
    char * t;
    int len;
    int ptr;
} Tape;

typedef struct {
    char state;
    char symbol;
    char toWrite;
    char direction;
    char nextState;
} Transition;

typedef struct {
    Transition * c;
    int len;
    int capacity;
} Code;

static Tape tape = {
    .t   = 0,
    .len = 0,
};

static Code code = {
    .c        = 0,
    .len      = 0,
    .capacity = 0,
};

static int  init(void);
static int  readTape(const char * file);
static void zero(int from, int until);
static int  right(char toWrite);
static int  left (char toWrite);
static void printt(void);
static void printBlanks(int * blankCount);
static int  readCode(const char * file);
static int  run(void);
static int  addCode(Transition * t);
static Transition * get(int state, char c);

int init(void) {
    if (!tape.t) {
        tape.t = malloc(MEMUNIT);
        zero(0, MEMUNIT);
        if (!tape.t) {
            printf("out of memory!\n");
            return NOK;
        }
        tape.len = MEMUNIT;
    }
    if (!code.c) {
        code.c = malloc(MEMUNIT * sizeof(Transition));
        if (!code.c) {
            printf("out of memory!\n");
            return NOK;
        }
        code.len = 0;
        code.capacity = MEMUNIT;
    }
    return OK;
}

int readTape(const char * file) {
    FILE * fp;
    char ch;
    int pstate = 0;
    int myptr  = -1;

    fp = fopen(file, "r");
    if (!fp) {
        printf("Failed to read tape from file %s\n", file);
        return NOK;
    }
    if (OK != init()) return 1;
    tape.ptr = 0;
    while (EOF != (ch = fgetc(fp))) {
        if (ch < 0x21) {
            continue;
        }
        if (PTR_L == ch) {
            if (pstate != 0) {
                printf("Pointer error.\n");
            }
            pstate = 1;
        } else if (PRT_R == ch) {
            if (pstate != 2) {
                printf("Pointer error.\n");
            } else {
                myptr = tape.ptr - 1;
                pstate = 3;
            }
        } else {
            if (OK != right(ch)) {
                printf("Read tape failed.\n");
                break;
            }
            if (1 == pstate) {
                pstate = 2;
            } else if (2 == pstate) {
                printf("Pointer error.\n");
                break;
            }
        }
    }
    fclose(fp);

    if (myptr < 0) {
        printf("No initial pointer position.\n");
        return NOK;
    }
    tape.ptr = myptr;
    return OK;
}

static void zero(int from, int until) {
    int i;

    for (i = from; i < until; ++i) {
        tape.t[i] = BLANK;
    }
}

static int right(char toWrite) {
    if (tape.ptr >= tape.len) {
        tape.t = realloc(tape.t, tape.len + MEMUNIT);
        if (!tape.t) {
            printf("Out of memory!\n");
            return NOK;
        }
        zero(tape.len, tape.len + MEMUNIT);
        tape.len += MEMUNIT;
    }
    tape.t[tape.ptr++] = toWrite;
    return OK;
}

static int left(char toWrite) {
    if (tape.ptr <= 0) {
        char * tmp = malloc(tape.len + MEMUNIT);
        if (!tmp) {
            printf("Out of memory!\n");
            return NOK;
        }
        memcpy(tmp + MEMUNIT, tape.t, tape.len);
        free(tape.t);
        tape.t = tmp;
        zero(0, MEMUNIT);
        tape.len += MEMUNIT;
        tape.ptr += MEMUNIT;
    }
    tape.t[tape.ptr--] = toWrite;
    return OK;
}

static void printBlanks(int * blankCount) {
    if (*blankCount) {
        if (*blankCount < BLANKREP) {
            int j;

            for (j = 0; j < *blankCount; ++j) {
                putchar('_');
                putchar(' ');
            }
        } else {
            printf("(%d x _) ", *blankCount);
        }
        *blankCount = 0;
    }
}

static void printt() {
    int i;
    int blankCount = 0;

    for (i = 0; i < tape.len; ++i) {
        if (tape.ptr == i) {
            printBlanks(&blankCount);
            printf("[%c] ", tape.t[i]);
        } else if (BLANK == tape.t[i]) {
            ++blankCount;
        } else {
            printBlanks(&blankCount);
            putchar(tape.t[i]);
            putchar(' ');
        }
    }
    if (blankCount) {
        int j;

        for (j = 0; j < BLANKREP && j < blankCount; ++j) {
            putchar('_');
            putchar(' ');
        }
    }
    putchar('\n');
}

static int readCode(const char * file) {
    FILE * fp;
    char cbuf[BUFSIZ];
    int state = 0;
    Transition t;

    fp = fopen(file, "r");
    if (!fp) {
        printf("Failed to read code from file %s\n", file);
        return NOK;
    }
    if (OK != init()) return 1;
    while (EOF != fscanf(fp, "%s", cbuf)) {
        switch (state) {
            case 0:
            {
                int s;

                sscanf(cbuf, "%d", &s);
                t.state = s;
                ++state;
                break;
            }
            case 1:
                ++state;
                t.symbol = cbuf[0];
                break;
            case 2:
                ++state;
                t.toWrite = cbuf[0];
                break;
            case 3:
                ++state;
                if (RIGHT == cbuf[0] || LEFT == cbuf[0]) {
                    t.direction = cbuf[0];
                } else {
                    printf("Read error - direction must be either '%c' or '%c'\n",
                            LEFT, RIGHT);
                    fclose(fp);
                    return NOK;
                }
                break;
            case 4:
            {
                int s;

                sscanf(cbuf, "%d", &s);
                t.nextState = s;
                if (OK != addCode(&t)) {
                    fclose(fp);
                    return NOK;
                }
                state = 0;
                break;
            }
            default:
                printf("Read code internal error\n");
                fclose(fp);
                return NOK;
        }
    }
    fclose(fp);
    return OK;
}

static int addCode(Transition * t) {
    if (code.len >= code.capacity) {
        code.c = realloc(code.c, (code.capacity + MEMUNIT) * sizeof(Transition));
        if (!code.c) {
            printf("Out of memory!\n");
            return NOK;
        }
        code.capacity += MEMUNIT;
    }
    memcpy(&code.c[code.len++], t, sizeof(Transition));
    return OK;
}

static Transition * get(int state, char c) {
    int i;

    for (i = 0; i < code.len; ++i) {
        if (code.c[i].state == state && code.c[i].symbol == c) {
            return &code.c[i];
        }
    }
    return 0;
}

static int run(void) {
    int state = 0;
    int cc = 0;

    while (1) {
        char c = tape.t[tape.ptr];
        Transition * t = get(state, c);

#ifdef STEP_BY_STEP_DEBUG
        printf("State: %d\n", state);
        printt();
        printf("\n");
        getchar();
#endif // STEP_BY_STEP_DEBUG

        if (!t) {
            printf("Iteration %d: No transition found for"\
                    " state %d symbol %c. Exiting\n", cc, state, c);
            break;
        }
        if (LEFT == t->direction) {
            if (OK != left(t->toWrite)) {
                return NOK;
            }
        } else {
            if (OK != right(t->toWrite)) {
                return NOK;
            }
        }
        state = t->nextState;
        if (state < 0) {
            break;
        }
        ++cc;
    }
    return OK;
}


int main(int argc, char ** argv) {
    if (argc != 3) {
        printf("Usage: %s TAPEFILE, CODEFILE\n", argv[0]);
        return 1;
    }
    if (OK != readTape(argv[1])) {
        return 1;
    }
    if (OK != readCode(argv[2])) {
        return 1;
    }

    if (OK != run()) {
        return 1;
    }
    printt();
    return 0;
}
