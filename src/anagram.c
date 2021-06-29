/**
 * @file anagram.c
 * @author maszynatrurla
 * @brief Programming exercise: Anagram finder
 *
 * Inspiration: http://programmingpraxis.com/2009/04/10/anagrams/
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DICTFILE        "dat/words.txt"

#define PAGE    100

#define OK      0
#define NOK     1

typedef struct Record_t {
    char *     letters;
    char **    words;
    int        capacity;
    int        length;
    struct Record_t * next;
} Record;

static int createDict(void);
static void getLetters(const char * word, char * buf);
static int addWord(const char * letters, const char * word);
static int alloCopy(char ** buf, const char * txt);
static int addWordToRecord(Record * r, const char * word);
static int printAnagramsTo(const char * word);

static Record * root = 0;
static int maxWords = 0;
static Record * maxRecord = 0;

int createDict() {
    FILE * fp;
    char buf[BUFSIZ];
    char let[BUFSIZ];

    fp = fopen(DICTFILE, "r");
    if (!fp) {
        printf("Failed to open word list at %s\n", DICTFILE);
        return NOK;
    }
    while (1 == fscanf(fp, "%s", buf)) {
        getLetters(buf, let);
        if (OK != addWord(let, buf)) {
            break;
        }
        if (ferror(fp) || feof(fp)) {
            break;
        }
    }

    fclose(fp);
    return OK;
}

void getLetters(const char * word, char * buf) {
    int rapid [127] = {0,};
    int i, k = 0;

    for (i = 0; word[i]; ++i) {
        ++rapid[(int) word[i]];
    }
    for (i = 0; i < sizeof(rapid) / sizeof(int); ++i) {
        int j;

        for (j = 0; j < rapid[i]; ++j) {
            buf[k++] = i;
        }
    }
    buf[k] = 0;
}

int addWord(const char * letters, const char * word) {
    Record * r, * pr = 0;

    for (r = root; r; r = r->next) {
        if (0 == strcmp(letters, r->letters)) {
            break;
        }
        pr = r;
    }

    if (!r) {
        if (pr) {
            pr->next = malloc(sizeof(Record));
            r = pr->next;
        } else {
            root = malloc(sizeof(Record));
            r = root;
        }
        if (!r) {
            printf("Out of memory!\n");
            return NOK;
        }
        r->next = 0;
        if (OK != alloCopy(&r->letters, letters)) {
            return NOK;
        }
        r->words = 0;
        r->capacity = 0;
        r->length = 0;
    }
    return addWordToRecord(r, word);
}

int addWordToRecord(Record * r, const char * word) {
    if (r->length >= r->capacity) {
        if (!r->words) {
            r->words = malloc(PAGE * sizeof(char *));
        } else {
            r->words = realloc(r->words, (r->capacity + PAGE) * sizeof(char *));
        }
        if (!r->words) {
            printf("Out of memory!\n");
            return NOK;
        }
        r->capacity += PAGE;
    }
    if (OK != alloCopy(&r->words[r->length++], word)) {
        return NOK;
    }
    if (maxWords < r->length) {
        maxWords = r->length;
        maxRecord = r;
    }
    return OK;
}

int alloCopy(char ** buf, const char * txt) {
    *buf = malloc(strlen(txt));
    if (!*buf) {
        printf("Out of memory!\n");
        return NOK;
    }
    strcpy(*buf, txt);
    return OK;
}

int printAnagramsTo(const char * word) {
    char * lbuf = malloc(strlen(word) + 1);
    Record * r;

    if (!lbuf) {
        printf("Out of memory!\n");
        return NOK;
    }
    getLetters(word, lbuf);

    for (r = root; r; r = r->next) {
        if (0 == strcmp(lbuf, r->letters)) {
            int j;

            printf("%d anagrams to %s found:\n", r->length, word);
            for (j = 0; j < r->length; ++j) {
                printf("\t%s\n", r->words[j]);
            }
            return OK;
        }
    }
    free(lbuf);
    printf("No anagrams found for %s\n", word);

    return NOK;
}


int main(int argc, char ** argv) {
    if (2 != argc) {
        printf("Usage:\n%s WORD\n", argv[0]);
        return 1;
    }
    if (OK != createDict()) {
        return 1;
    }

    printAnagramsTo(argv[1]);

    printf("Maximal number of anagrams found in dictionary is %d:\n",
            maxWords);
    if (maxRecord) {
        int i;

        for (i = 0; i < maxRecord->length; ++i) {
            printf("\t%s\n", maxRecord->words[i]);
        }
    }

    return 0;
}
