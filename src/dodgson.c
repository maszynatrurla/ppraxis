/**
 * @file dodgson.c
 * @author maszynatrurla
 * @brief Programming exercise: Dodgson Doublets
 * 
 * Inspiration: http://programmingpraxis.com/2009/03/20/dodgsons-doublets/
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define DICTFILE        "dat/words.txt"

#define OK      0
#define NOK     1

#define PAGE    100

typedef struct {
    char ** words;
    int length;
    int capacity;
} Dict;

typedef struct Stacked_t {
    Dict found;
    int  i;
    struct Stacked_t * prev;
} Stacked;

static int createDict(int wlen);
static int dictInit(void);
static int dictAdd(const char * txt, int wlen);
static void play(char * A, char * B);
static int find(Stacked ** s, const char * txt);
static void prints(Stacked * s);
static int invert(Stacked ** s);
static void pop(Stacked * s);
static int findWords(Stacked ** s, const char * txt);
static int cmpWords(Stacked * d, const char * txt);

static Dict dict = {
    .words = 0,
    .length = 0,
    .capacity = 0,
};

int createDict(int wlen) {
    FILE * fp;
    char buf[BUFSIZ];
    
    if (OK != dictInit()) {
        return NOK;
    }
    
    fp = fopen(DICTFILE, "r");
    if (!fp) {
        printf("Failed to open dictionary file at location: %s\n", DICTFILE);
        return NOK;
    }
    
    while (!feof(fp) && !ferror(fp)) {
        if (1 != fscanf(fp, "%s", buf)) {
            break;
        }
        if (strlen(buf) == wlen) {
            if (OK != dictAdd(buf, wlen)) {
                break;
            }
        }
    }
    
    fclose(fp);
    
    return OK;
}

int dictInit(void) {
    if (!dict.words) {
        dict.words = malloc(PAGE * sizeof(char *));
        if (!dict.words) {
            printf("Out of memory!\n");
            return NOK;
        }
        dict.capacity = PAGE;
    }
    return OK;
}

int dictAdd(const char * txt, int wlen) {
    if (dict.length >= dict.capacity) {
        dict.words = realloc(dict.words, (dict.capacity + PAGE) * sizeof(char *));
        if (!dict.words) {
            printf("Failed to allocate memory!\n");
            return NOK;
        }
        dict.capacity += PAGE;
    }
    dict.words[dict.length] = malloc((wlen + 1) * sizeof(char));
    if (!dict.words[dict.length]) {
        printf("Out of memory!\n");
        return NOK;
    }
    strcpy(dict.words[dict.length], txt);
    ++dict.length;
    return OK;
}

void play(char * A, char * B) {
    char * cA [] = {A,};
    char * cB [] = {B,};
    Stacked sA = {
        .found  = {cA, 1, 1},
        .i      = 0,
        .prev   = 0,
    };
    Stacked sB = {
        .found  = {cB, 1, 1},
        .i      = 0,
        .prev   = 0,
    };
    Stacked * aA = &sA;
    Stacked * aB = &sB;
    
    while (1) {
        if (aA && find(&aA, B)) {
            invert(&aA);
            prints(aA);
            break;
        }
        if (aB && find(&aB, A)) {
            prints(aB);
            break;
        }
        if (!aA && !aB) {
            printf("Failed to find link between %s and %s\n", A, B);
            break;
        }
    }
    
}

void prints(Stacked * s) {
    if (s) {
        int i;
        for (i = 0; s->found.words[s->i][i]; ++i) {
            putchar(toupper(s->found.words[s->i][i]));
        }
        putchar('\n');
    }
    for (s = s->prev; s; s = s->prev) {
        if (s->prev) {
            printf("%s\n", s->found.words[s->i]);
        } else {
            int i;
            
            for (i = 0; s->found.words[s->i][i]; ++i) {
                putchar(toupper(s->found.words[s->i][i]));
            }
            putchar('\n');
        }
    }
    
}

int invert(Stacked ** s) {
    Stacked * n = 0, *pn = 0, *ps = 0;
    
    for (; *s; *s = (*s)->prev) {
        n = malloc(sizeof(Stacked));
        if (!n) {
            printf("Out of memory\n");
            return NOK;
        }
        n->prev = pn;
        n->i = 0;
        n->found.words = malloc(sizeof(char *));
        if (!n->found.words) {
            printf("Out of memory\n");
            return NOK;
        }
        n->found.length = 1;
        n->found.capacity = 1;
        n->found.words[0] = (*s)->found.words[(*s)->i];
        
        if (ps) {
            free(ps);
        }
        pn = n;
        ps = *s;
    }
    *s = n;
    return OK;
}

void pop(Stacked * s) {
    free(s->found.words);
    free(s);
}

void printss(Stacked * s) {
//     printf("--------------------------------------------------------------\n");
    int ss = 0;
    for (; s; s = s->prev) {
        ++ss;
    }
    printf("%d\n", ss);
}

int find(Stacked ** s, const char * txt) {
    Stacked * e = *s;
    
    if (e->i < e->found.length) {
        if (findWords(&e, e->found.words[e->i])) {
            if (cmpWords(e, txt)) {
                *s = e;
                return 1;
            }
        } else {
            ++e->i;
        }
    } else {
        Stacked * tmp = e->prev;
        pop(e);
        e = tmp;
        if (e) {
            ++(e->i);
        }
    }
    *s = e;
    return 0;
}

int cmpWords(Stacked * s, const char * txt) {
    int i;
    Dict * d = &s->found;    
    
    for (i = 0; i < d->length; ++i) {
        if (0 == strcmp(txt, d->words[i])) {
            s->i = i;
            return 1;
        }
    }
    return 0;
}
    
int findWords(Stacked ** ps, const char * txt) {
    int i;
    Stacked * s = *ps;
    Stacked * r = 0;
    
    for (i = 0; i < dict.length; ++i) {
        int j;
        int diffs = 0;
        
        for (j = 0; dict.words[i][j]; ++j) {
            if (dict.words[i][j] != txt[j]) {
                ++diffs;
            }
        }
        if (1 == diffs) {
            Stacked * e;
            int ok = 1;
            
            for (e = s; e; e = e->prev) {
                if (0 == strcmp(e->found.words[e->i], dict.words[i])) {
                    ok = 0;
                    break;
                }
            }
            if (ok) {
                if (!r) {
                    r = malloc(sizeof(Stacked));
                    if (!r) {
                        printf("Out of memory!");
                        return 0;
                    }
                    r->prev = s;
                    r->i = 0;
                    r->found.words = malloc(PAGE * sizeof(char *));
                    r->found.length = 0;
                    r->found.capacity = PAGE;
                }
                if (r->found.length >= r->found.capacity) {
                    r->found.words = realloc(r->found.words,
                            (r->found.capacity + PAGE) * sizeof(char *));
                    if (!r->found.words) {
                        printf("Out of memory\n");
                        return 0;
                    }
                    r->found.capacity += PAGE;
                }
                r->found.words[r->found.length++] = dict.words[i];
            }
        }
    }
    if (r) {
        *ps = r;
        return 1;
    }
    return 0;
}

int main(int argc, char ** argv) {
    int wlen;
    int i;
    
    if (3 != argc) {
        printf("Usage: %s WORD_A WORD_B\n", argv[0]);
        return 1;
    }
    wlen = strlen(argv[1]);
    if (strlen(argv[2]) != wlen) {
        printf("Words have different lengths\n");
        return 1;
    }
    for (i = 0; argv[1][i]; ++i) argv[1][i] = tolower(argv[1][i]);
    for (i = 0; argv[2][i]; ++i) argv[2][i] = tolower(argv[2][i]);
    
    if (OK != createDict(wlen)) {
        printf("Failed to initiate dictionary.\n");
        return 1;
    }
    
    play(argv[1], argv[2]);
    

    return 0;
}
