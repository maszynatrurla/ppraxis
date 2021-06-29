/**
 * @file bloomspell.c
 * @author maszynatrurla
 * @brief Programming exercise: Spell checker using bloom filter.
 * 
 * Inspiration: http://programmingpraxis.com/2009/04/21/probabilistic-spell-checking/
 */

#include <stdio.h>
#include <ctype.h>

#define DICTFILE        "dat/words.txt"

#define EM    1000000
#define KA      7

#define OK      0
#define NOK     1

static int createDict(void);
static unsigned hash(const char * word, int k);
static void addWord(const char * word);
static void check(const char * file);
static int  checkWord(const char * word);

static int bits [EM] = {0};

int createDict() {
    FILE * fp;
    char buf[BUFSIZ];
    
    fp = fopen(DICTFILE, "r");
    if (!fp) {
        printf("Failed to open dictionary file %s\n", DICTFILE);
        return NOK;
    }
    
    while (1 == fscanf(fp, "%s", buf)) {
        addWord(buf);
    }
    
    
    fclose(fp);
    return OK;
}

void addWord(const char * word) {
    int i;
    
    for (i = 0; i < KA; ++i) {
        unsigned j;
        unsigned h = hash(word, i) % EM;
        
        for (j = h; j < EM; j += h) {
            bits[j] = 1;
        }
    }
}


unsigned hash(const char * word, int k) {
    /* sdbm */
    unsigned h = 0;
    int i;
    
    if (k) {
        h = (97 + k) + (h << 6) + (h << 16) - h;
    }
    
    for (i = 0; word[i]; ++i) {
        char c = tolower(word[i]);
        
        h = c + (h << 6) + (h << 16) - h;
    }
    
    if (k) {
        h = (97 + k) + (h << 6) + (h << 16) - h;
    }
    
    return h;
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
    int i;
    
    for (i = 0; i < KA; ++i) {
        unsigned j;
        unsigned h = hash(word, i) % EM;
        
        for (j = h; j < EM; j += h) {
            if (!bits[j]) {
                return NOK;
            }
        }
    }
    
    return OK;
}

int main(int argc, char ** argv) {
//     int i;
    if (2 != argc) {
        printf("Usage:\n%s FILE\n", argv[0]);
        return 1;
    }
    if (OK != createDict()) {
        return 1;
    }
    
    check(argv[1]);
//     for (i = 0; i < EM; ++i) {
//         if (0 == i % 50) {
//             putchar('\n');
//         }
//         printf("%d ", bits[i]);
//     }
    
    return 0;
}
