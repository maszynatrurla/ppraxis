/**
 * @file spellcheck.c
 * @author maszynatrurla
 * @brief Programming exercise: Simple spell checker.
 * 
 * Inspiration: http://programmingpraxis.com/2009/04/17/spell-checking/
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define DICTFILE        "dat/words.txt"

#define PAGE    100

#define OK      0
#define NOK     1

typedef struct Node_t {
    char C;
    char word;
    struct Node_t * followups;
    int length;
    int capacity;
} Node;

static int  createTrie(void);
static void check(const char * file);
static int  addWord(const char * word);
static int  alocTrie(Node * node);
static int  ensureTrie(Node * node);
static Node * findTrie(Node * parent, char c);
static Node * appendTrie(Node * parent, char c);
static int  checkWord(const char * word);
static int  isWordEnd(const char * word, int idx);
static int  isWordStart(char c);

static Node trie = {
    .followups = 0,
    .length    = 0,
    .capacity  = 0,
    .word      = 0,
};

static const char * ignoreWhenOnEnd = ",.;!?):\"']}";
static const char * ignoreWhenOnStart = "'\"[{(";

int alocTrie(Node * node) {
    node->followups = malloc(PAGE * sizeof(Node));
    if (!node->followups) {
        printf("Out of memory\n");
        return NOK;
    }
    node->length   = 0;
    node->capacity = PAGE;
    node->word     = 0;
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

int createTrie() {
    FILE * fp;
    char buf[BUFSIZ];
    
    fp = fopen(DICTFILE, "r");
    if (!fp) {
        printf("Failed to open dictionary file %s\n", DICTFILE);
        return NOK;
    }
    
    alocTrie(&trie);
    
    while (1 == fscanf(fp, "%s", buf)) {
        if (OK != addWord(buf)) {
            break;
        }
    }
    
    
    fclose(fp);
    return OK;
}

int addWord(const char * word) {
    int wi;
    Node * n = &trie;
    
    for (wi = 0; word[wi]; ++wi) {
        char c = tolower(word[wi]);
        Node * nn = findTrie(n, c);
        
        if (!nn) {
            nn = appendTrie(n, c);
        }
        if (!nn) {
            return NOK;
        }
        n = nn;
    }
    n->word = 1;

    return OK;
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

void check(const char * file) {
    FILE * fp;
    char buf[BUFSIZ];
    int totals = 0, errors = 0;
    
    fp = fopen(file, "r");
    if (!fp) {
        printf("Could not read file %s\n", file);
        return;
    }
    
    while (1 == fscanf(fp, "%s", buf)) {
        if (OK != checkWord(buf)) {
            printf("Possible spelling error: \"%s\"\n", buf);
            ++errors;
        }
        ++totals;
    }
    
    fclose(fp);
    
    printf("Spellcheck of file %s complete.\n", file);
    printf("\tTotal words read: %d\n", totals);
    printf("\tErrors found: %d\n", errors);
}

int checkWord(const char * word) {
    int wi;
    Node * n = &trie;
    
    for (wi = 0; NOK == isWordEnd(word, wi); ++wi) {
        char c = tolower(word[wi]);
        
        if (!wi && NOK == isWordStart(c)) {
            continue;
        }
        if (c > 0x29 && c < 0x40) {
            return OK;
        }
        
        n = findTrie(n, c);
        
        if (!n) {
            return NOK;
        }
    }
    return n->word ? OK : NOK;
}

int isWordEnd(const char * word, int idx) {
    if (!word[idx]) {
        return OK;
    }
    if (!word[idx + 1]) {
        int i;
        
        for (i = 0; ignoreWhenOnEnd[i]; ++i) {
            if (ignoreWhenOnEnd[i] == word[idx]) {
                return OK;
            }
        }
    }
    
    return NOK;
}

int isWordStart(char c) {
    int i;
    
    for (i = 0; ignoreWhenOnStart[i]; ++i) {
        if (ignoreWhenOnStart[i] == c) {
            return NOK;
        }
    }
    return OK;
}

int main(int argc, char ** argv) {
    if (2 != argc) {
        printf("Usage:\n%s FILE\n", argv[0]);
        return 1;
    }
    if (OK != createTrie()) {
        return 1;
    }
    
    check(argv[1]);

    return 0;
}
