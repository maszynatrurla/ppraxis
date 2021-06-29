/**
 * @file hyphen.c
 * @author maszynatrurla
 * @brief Programming exercise: Automatic hyphenation.
 * 
 * Inspiration: http://programmingpraxis.com/2009/04/24/word-hy-phen-a-tion-by-com-pu-ter/
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OK      0
#define NOK     1

#define PAGE    100

#define PATTERNFILE     "dat/hyphens"
#define EXCEPTIONFILE   "dat/exceptions"

typedef struct Node_t {
    char C;
    int end;
    struct Node_t * followups;
    int length;
    int capacity;
} Node;

typedef struct Indices_t {
    char * coefs;
    char * wheres;
    int length;
    int capacity;
} Indices;

typedef struct {
    char ** words;
    int * indices;
    int length;
    int capacity;
} Exceptions;

static int readPatterns(void);
static int readExceptions(void);
static int alocTrie(Node * node);
static int ensureTrie(Node * node);
static int addPattern(const char * p);
static Node * findTrie(Node * parent, char c);
static Node * appendTrie(Node * parent, char c);
static int addIdx(char coef, char idx);
static void hyphenate(const char * word);
static void hyph(const char * w, int idx, char * hyspace);
static int addException(const char * word);
static int alocExceptions(void);
static int findException(const char * word);
static void printException(int i);

static Node trie = {
    .followups = 0,
    .length    = 0,
    .capacity  = 0,
    .end       = -1,
};

static Indices indices = {
    .coefs  = 0,
    .wheres = 0,
    .length = 0,
    .capacity = 0,
};

static Exceptions exceptions = {
    .words = 0,
    .indices = 0,
    .length = 0,
    .capacity = 0,
};

int addIdx(char coef, char idx) {
    if (!indices.coefs) {
        indices.coefs = malloc(PAGE);
        indices.wheres = malloc(PAGE);
        if (!indices.coefs || !indices.wheres) {
            printf("Out of memory!\n");
            return NOK;
        }
        indices.length = 0;
        indices.capacity = PAGE;
    }
    if (indices.length >= indices.capacity) {
        indices.coefs = realloc(indices.coefs, indices.capacity + PAGE);
        indices.wheres = realloc(indices.wheres, indices.capacity + PAGE);
        if (!indices.coefs || !indices.wheres) {
            printf("Out of memory!\n");
            return NOK;
        }
        indices.capacity += PAGE;
    }
    indices.coefs[indices.length] = coef;
    indices.wheres[indices.length] = idx;
    ++indices.length;
    
    return OK;
}

int alocTrie(Node * node) {
    node->followups = malloc(PAGE * sizeof(Node));
    if (!node->followups) {
        printf("Out of memory\n");
        return NOK;
    }
    node->length   = 0;
    node->capacity = PAGE;
    node->end      = -1;
    return OK;
}

int ensureTrie(Node * node) {
    if (node->length >= node->capacity) {
        node->followups = realloc(node->followups,
                                  (node->capacity + PAGE) * sizeof(Node));
        if (!node->followups) {
            printf("Out of memory!\n");
            return NOK;
        }
        node->capacity += PAGE;
    }
    return OK;
}

int addPattern(const char * p) {
    int wi;
    Node * n = &trie;
    int tmp = indices.length;
    int gi = 0;
    
    for (wi = 0; p[wi]; ++wi) {
        if (p[wi] >= '0' && p[wi] <= '9') {
            addIdx(p[wi], gi);
            continue;
        } else {
            ++gi;
        }
        Node * nn = findTrie(n, p[wi]);
        
        if (!nn) {
            nn = appendTrie(n, p[wi]);
        }
        if (!nn) {
            return NOK;
        }
        n = nn;
    }
    addIdx(0, 0);
    n->end = tmp;

    return OK;
}

void hyph(const char * w, int idx, char * hyspace) {
    char c;
    Node * n = &trie;
    int start = idx;
    
    if (idx < 0) {
        c = '.';
    } else {
        c = w[idx];
    }
    
    for (; c; ) {
        n = findTrie(n, c);
        if (!n) {
            break;
        }
        if (n->end >= 0) {
            int i;
            
            for (i = n->end; indices.coefs[i]; ++i) {
                char coef = indices.coefs[i] - 0x30;
                
                
                if (hyspace[start + (int) indices.wheres[i]] < coef) {
                    hyspace[start + (int) indices.wheres[i]] = coef;
                }
            }
        }
        
        c = w[++idx];
    }
    
    if (n) {
        n = findTrie(n, '.');
        if (n && n->end >= 0) {
            int i;
            
            for (i = n->end; indices.coefs[i]; ++i) {
                char coef = indices.coefs[i] - 0x30;
                
                if (hyspace[start + (int) indices.wheres[i]] < coef) {
                    hyspace[start + (int) indices.wheres[i]] = coef;
                }
            }
        }
    }
}

void hyphenate(const char * word) {
    int i;
    char * hyspace = calloc(strlen(word), 1);
    int l = strlen(word);
    
    if (0 <= (i = findException(word))) {
        printException(i);
        return;
    }
    
    if (!hyspace) {
        printf("Out of memory!\n");
        return;
    }
    
    for (i = -1; i < l; ++i) {
        hyph(word, i, hyspace);
    }
    
    for(i = 0; word[i]; ++i) {
        if (i != 0 && i != l - 1 && hyspace[i] && hyspace[i] % 2) {
            putchar('-');
        }
        putchar(word[i]);
    }
    putchar('\n');
}

int findException(const char * word) {
    int i;
    
    for (i = 0; i < exceptions.length; ++i) {
        if (0 == strcmp(word, exceptions.words[i])) {
            return i;
        }
    }
    return -1;
}

void printException(int i) {
    const char * word = exceptions.words[i];
    int j;
    
    i = exceptions.indices[i];
    for (j = 0; word[j]; ++j) {
        if (indices.coefs[i] && indices.wheres[i] == j) {
            putchar('-');
            ++i;
        }
        putchar(word[j]);
    }
    putchar('\n');
}

Node * findTrie(Node * parent, char c) {
    int i;
    
    for (i = 0; i < parent->length; ++i) {
        if (c == parent->followups[i].C) {
            return &parent->followups[i];
        }
    }
    return 0;
}

Node * appendTrie(Node * parent, char c) {
    Node * node = 0;
    
    if (OK != ensureTrie(parent)) {
        return 0;
    }
    
    node = &parent->followups[parent->length++];
    if (OK != alocTrie(node)) {
        return 0;
    }
    node->C = c;
    
    return node;
}

int readPatterns() {
    FILE * fp;
    char buf[BUFSIZ];
    
    if (OK != alocTrie(&trie)) {
        return 1;
    }
    
    fp = fopen(PATTERNFILE, "r");
    if (!fp) {
        printf("Failed to open pattern file %s\n", PATTERNFILE);
        return NOK;
    }
    
    while (1 == fscanf(fp, "%s", buf)) {
        if (OK != addPattern(buf)) {
            break;
        }
    }
    
    fclose(fp);
    return OK;
}

int readExceptions() {
    FILE * fp;
    char buf[BUFSIZ];
    
    if (OK != alocExceptions()) {
        return NOK;
    }
    
    fp = fopen(EXCEPTIONFILE, "r");
    if (!fp) {
        printf("Failed to open exceptions file %s\n", EXCEPTIONFILE);
        return NOK;
    }
    
    while (1 == fscanf(fp, "%s", buf)) {
        if (OK != addException(buf)) {
            break;
        }
    }
    
    fclose(fp);
    return OK;
}

int alocExceptions() {
    exceptions.words = malloc(PAGE * sizeof(char *));
    exceptions.indices = malloc(PAGE * sizeof(int));
    if (!exceptions.words || !exceptions.indices) {
        printf("Out of memory!\n");
        return NOK;
    }
    exceptions.length = 0;
    exceptions.capacity = PAGE;
    return OK;
}

int addException(const char * word) {
    int i, l = 0;
    
    if (exceptions.length >= exceptions.capacity) {
        exceptions.words = realloc(exceptions.words,
                (exceptions.capacity + PAGE) * sizeof(char *));
        exceptions.indices = realloc(exceptions.indices,
                (exceptions.capacity + PAGE) * sizeof(int));
        if (!exceptions.words || !exceptions.indices) {
            printf("Out of memory!\n");
            return NOK;
        }
        exceptions.capacity += PAGE;
    }
    
    exceptions.indices[exceptions.length] = indices.length;
    
    for (i = 0; word[i]; ++i) {
        if ('-' == word[i]) {
            addIdx('-', l);
        } else {
            ++l;
        }
    }
    
    exceptions.words[exceptions.length] = malloc(l + 1);
    if (!exceptions.words[exceptions.length]) {
        printf("Out of memory!\n");
        return NOK;
    }
    l = 0;
    for (i = 0; word[i]; ++i) {
        if ('-' != word[i]) {
            exceptions.words[exceptions.length][l++] = word[i];
        }
    }
    exceptions.words[exceptions.length][l] = 0;
    
    ++exceptions.length;
    return OK;
}

int main(int argc, char ** argv) {
    if (argc < 2) {
        printf("Usage:\n%s WORD\n", argv[0]);
        return 1;
    }
    if (OK != readPatterns()) {
        return 1;
    }
    if (OK != readExceptions()) {
        return 1;
    }
    
    hyphenate(argv[1]);

    return 0;
}
