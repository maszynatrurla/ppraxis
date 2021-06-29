/**
 * @file words.c
 * @author maszynatrurla
 * @brief Programming exercise: Find word frequencies.
 * 
 * Inspiration: http://programmingpraxis.com/2009/03/10/word-frequencies/
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define OK      0
#define NOK     1

typedef struct WordLL_t {
    char * word;
    int count;
    struct WordLL_t * next;
} WordLL;

static void readWords(const char * file);
static void writeWords(int N);
static int  addWord(const char * word);
static void delWord(WordLL * prev);

static WordLL * root = 0;

void readWords(const char * file) {
    FILE * fp;
    
    fp = fopen(file, "r");
    if (!fp) {
        printf("Failed to open file %s\n", file);
        return;
    }
    
    while (1) {
        char buf[BUFSIZ];
        
        if (feof(fp) || ferror(fp)) {
            break;
        }
        
        if (1 != fscanf(fp, "%s", buf)) {
            break;
        }
        
        if (OK != addWord(buf)) {
            return;
        }
    }
    
    fclose(fp);
}

int addWord(const char * word) {
    WordLL * w;
    
    for (w = root; w; w = w->next) {
        if (0 == strcmp(word, w->word)) {
            ++w->count;
            return OK;
        }
        if (!w->next) {
            break;
        }
    }
    if (root) {
        w->next = malloc(sizeof(WordLL));
        w = w->next;
    } else {
        root = malloc(sizeof(WordLL));
        w = root;
    }
    
    if (!w) {
        printf("Out of memory!\n");
        return NOK;
    }
    w->next = 0;
    w->count = 1;
    w->word = malloc(strlen(word) + 1);
    if (!w->word) {
        printf("Out of memory!\n");
        return NOK;
    }
    strcpy(w->word, word);
    
    return OK;
}

void writeWords(int N) {
    int i;
    
    for (i = 0; i < N; ++i) {
        int max = 0;
        WordLL * w, *pw = 0, *maxw = 0;
        
        if (!root) {
            break;
        }
        
        for (w = root; w; w = w->next) {
            if (w->count > max) {
                maxw = pw;
                max = w->count;
            }
            pw = w;
        }
        if (maxw) {
            w = maxw->next;
        } else {
            w = root;
        }
        printf("%-32s%d\n", w->word, w->count);
        delWord(maxw);
    }
}

void delWord(WordLL * prev) {
    WordLL * tmp;
    
    if (prev) {
        tmp = prev->next;
        prev->next = tmp->next;
    } else {
        tmp = root;
        root = root->next;
    }
    free(tmp->word);
    free(tmp);
}

int main(int argc, char ** argv) {
    int N;
    
    if (argc != 3) {
        printf("Usage: %s FILENAME N\n", argv[0]);
        return 1;
    }
    
    sscanf(argv[2], "%d", &N);
    
    readWords(argv[1]);
    writeWords(N);
    
    return 0;
}


