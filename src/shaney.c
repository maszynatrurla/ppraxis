/**
 * @file shaney.c
 * @author maszynatrurla
 * @brief Programming exercise: Generate parodies of text.
 * 
 * Inspiration: http://programmingpraxis.com/2009/02/27/mark-v-shaney/
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MODE_INCORRECT  0
#define MODE_LEARN      1
#define MODE_WRITE      2
#define MODE_DATABASE   4

#define OK      0
#define NOK     1

#define MAX_WORDS       200
#define MIN_WORDS       80

#define MAX_TRIES       5000

typedef char * Txt;

typedef struct {
    int mode;
    Txt database;
    Txt input;
} Options;

typedef struct ThirdLL_t {
    Txt word;
    int count;
    struct ThirdLL_t * next;
} ThirdLL;

typedef struct {
    Txt first;
    Txt second;
    int count;
    ThirdLL * follows;
} Record;

typedef struct RecordLL_t {
    Record record;
    struct RecordLL_t * next;
} RecordLL;

static void parseArgs(int argc, char ** argv);
static void usage    (Txt self);
static int  readDb   (Txt file);
static int  writeDb  (Txt file);
static int  learn    (Txt file);
static void write    (void);
static void freeDb   (void);
static int  process  (Txt first, Txt second, Txt third);
static int  alocCopy (Txt source, Txt * dest);
static RecordLL * newRecord(Txt first, Txt second, Txt third, int count, int thirdCount);
static RecordLL * getRecord(int index);
static Txt chooseThird(ThirdLL * words);
static RecordLL * markovNext(Txt first, Txt second);

static Options options = {
    .mode     = MODE_INCORRECT,
    .database = 0,
    .input    = 0,
};

static RecordLL * root = 0;
static RecordLL * last = 0;
static int allCount = 0;

void parseArgs(int argc, char ** argv) {
    int i;
    int state = MODE_INCORRECT;
    
    for (i = 1; i < argc; ++i) {
        switch (state) {
        case MODE_DATABASE:
            state = MODE_INCORRECT;
            options.database = argv[i];
            break;
        case MODE_LEARN:
            state = MODE_INCORRECT;
            options.input = argv[i];
            break;
        default:
            if (0 == strcmp("use", argv[i])) {
                state = MODE_DATABASE;
                options.mode |= MODE_DATABASE;
            } else if (0 == strcmp("learn", argv[i])) {
                state = MODE_LEARN;
                options.mode |= MODE_LEARN;
            } else if (0 == strcmp("write", argv[i])) {
                options.mode |= MODE_WRITE;
            } else {
                options.mode = MODE_INCORRECT;
                printf("Unknown command: %s\n", argv[i]);
                return;
            }
            break;
        }
    }
    
    if (MODE_INCORRECT == options.mode
            || MODE_LEARN == options.mode
            || MODE_WRITE == options.mode
            || MODE_DATABASE == options.mode) {
        options.mode = MODE_INCORRECT;
        printf("Nothing to do.\n");
        return;
    } else if ((MODE_DATABASE & options.mode) && 0 == options.database) {
        options.mode = MODE_INCORRECT;
        printf("No database file name given.\n");
        return;
    } else if ((MODE_INCORRECT & options.mode) && 0 == options.input) {
        options.mode = MODE_INCORRECT;
        printf("No input file name given.\n");
        return;
    }
}

void usage(Txt self) {
    printf("\nUsage:\n%s [use|learn|write] [inputFile|databaseFile]\n", self);
    printf("\nExamples:\n");
    printf("\t%s use baza.db learn tekst.txt\n", self);
    printf("\t%s use baza.db write\n", self);
    printf("\t%s learn tekst.txt write\n", self);
}

int  readDb(Txt file) {
    FILE * fp;
    char cbuf[3][BUFSIZ];
    int count;
    
    fp = fopen(file, "rb");
    if (!fp) {
        return NOK;
    }
    
    while (!feof(fp) && !ferror(fp)) {
        int i;
        int c;
        RecordLL * r;
        ThirdLL * t;
        
        if (EOF == fscanf(fp, "%s", cbuf[0])) {
            break;
        }
        fgetc(fp);
        if (EOF == fscanf(fp, "%s", cbuf[1])) {
            printf("Warning: database looks corrupted.\n");
            break;
        }
        fgetc(fp);
        if (1 != fread(&count, sizeof(int), 1, fp)) {
            printf("Warning: database looks corrupted.\n");
            break;
        }
        if (!count) {
            printf("Warning: database looks corrupted.\n");
            break;
        }
        if (EOF == fscanf(fp, "%s", cbuf[2])) {
            printf("Warning: database looks corrupted.\n");
            break;
        }
        fgetc(fp);
        if (1 != fread(&c, sizeof(int), 1, fp)) {
            printf("Warning: database looks corrupted.\n");
            break;
        }
        if (NULL == (r = newRecord(cbuf[0], cbuf[1], cbuf[2], count, c))) {
            break;
        }
        t = r->record.follows;
        for (i = 1; i < count; ++i) {
            if (EOF == fscanf(fp, "%s", cbuf[0])) {
                printf("Warning: database looks corrupted.\n");
                goto hell;
            }
            fgetc(fp);
            if (1 != fread(&c, sizeof(int), 1, fp)) {
                printf("Warning: database looks corrupted.\n");
                goto hell;
            }
            t->next = (ThirdLL *) malloc(sizeof(ThirdLL));
            if (!t->next) {
                printf("Out of memory!\n");
                goto hell;
            }
            t = t->next;
            t->next = 0;
            t->count = c;
            if (OK != alocCopy(cbuf[0], &t->word)) {
                goto hell;
            }
        }
        ++allCount;
    }
hell:    
    fclose(fp);
    
    return OK;
}

int  writeDb(Txt file) {
    RecordLL * r;
    FILE * fp;

    fp = fopen(file, "wb");
    if (!fp) {
        return NOK;
    }
    
    for (r = root; r; r = r->next) {
        ThirdLL * t;
        int i;
        
        fwrite(r->record.first, 1, strlen(r->record.first), fp);
        fputc(0x20, fp);
        fwrite(r->record.second, 1, strlen(r->record.second), fp);
        fputc(0x20, fp);
        fwrite(&r->record.count, sizeof(int), 1, fp);
        
        t = r->record.follows;
        for (i = 0; i < r->record.count && t; ++i) {
            fwrite(t->word, 1, strlen(t->word), fp);
            fputc(0x20, fp);
            fwrite(&t->count, sizeof(int), 1, fp);
            t = t->next;
        }
        
        if (ferror(fp)) {
            printf("Write error\n");
            break;
        }
    }
    
    fclose(fp);
    
    return OK;
}

int  learn(Txt file) {
    FILE * fp;
    char cbuf [3][BUFSIZ];
    int wrd = 0;
    
    cbuf[0][0] = 0;
    cbuf[1][0] = 0;
    cbuf[2][0] = 0;
        
    fp = fopen(file, "r");
    if (!fp) {
        return NOK;
    }
    
    while (EOF != fscanf(fp, "%s", cbuf[wrd++])) {
        int res = OK;
        
        if (wrd > 2) {
            wrd = 0;
        }
        if (0 == wrd) {
            res = process(cbuf[0], cbuf[1], cbuf[2]);
        } else if (1 == wrd) {
            res = process(cbuf[1], cbuf[2], cbuf[0]);
        } else {
            res = process(cbuf[2], cbuf[0], cbuf[1]);
        }
        if (feof(fp) || ferror(fp) || OK != res) {
            break;
        }
    }
    
    fclose(fp);
    
    return OK;
}

void write() {
    int targetLength;
    int start;
    RecordLL * r;
    Txt * words;
    int try = 0;
    int i, j;
    
#ifdef DROPDB
    for (r = root; r; r = r->next) {
        ThirdLL * t;
        
        printf("%s %s %d\n", r->record.first, r->record.second, r->record.count);
        for (t = r->record.follows; t; t = t->next) {
            printf("\t%s %d\n", t->word, t->count);
        }
    }
#endif
    
    srand(time(0));
    targetLength = MIN_WORDS + (rand() % (MAX_WORDS - MIN_WORDS));
    words = malloc((targetLength + 3) * sizeof(Txt));
    if (!words) {
        printf("Out of memory!\n");
        return;
    }
    
    while (1) {
        i = 0;
        
        if (try > MAX_TRIES) {
            printf("Failed after %d tries.\n", try);
            return;
        }
        ++try;
        start = rand() % allCount;
        r = getRecord(start);
        words[i++] = r->record.first;
        words[i++] = r->record.second;
        
        for (; i < targetLength;) {
            if (1 == r->record.count) {
                words[i++] = r->record.follows->word;
            } else {
                words[i++] = chooseThird(r->record.follows);
            }
            r = markovNext(words[i - 2], words[i - 1]);
            if (!r) {
                break;
            }
        }
        if (i >= MIN_WORDS) {
            break;
        }
    }
    
    printf("\n");
    for (j = 0; j < i; ++j) {
        printf("%s ", words[j]);
    }
    printf("\n\n");
    
    free(words);
}

void freeDb() {
    RecordLL * r, * pr = 0;
    
    for (r = root; r; r = r->next) {
        ThirdLL * t, * pt = 0;
        
        free(r->record.first);
        free(r->record.second);
        for (t = r->record.follows; t; t = t->next) {
            free(t->word);
            if (pt) {
                free(pt);
            }
            pt = t;
        }
        if (pt) {
            free(pt);
        }
        if (pr) {
            free(pr);
        }
        pr = r;
    }
    if (pr) {
        free(pr);
    }
}

int process(Txt first, Txt second, Txt third) {
    RecordLL * r, * pr = 0;
    if (!first[0] || !second[0] || !third[0]) {
        return OK;
    }
    for (r = root; r; r = r->next) {
        if (0 == strcmp(first, r->record.first)
                && 0 == strcmp(second, r->record.second)) {
            break;
        }
        pr = r;
    }
    if (r) {
        ThirdLL * t, * pt = 0;
        
        for (t = r->record.follows; t; t = t->next) {
            if (0 == strcmp(third, t->word)) {
                break;
            }
            pt = t;
        }
        if (t) {
            ++(t->count);
        } else {
            ++(r->record.count);
            pt->next = (ThirdLL *) malloc(sizeof(ThirdLL));
            pt = pt->next;
            if (!pt) {
                printf("Out of memory!\n");
                return NOK;
            }
            pt->next = 0;
            pt->count = 1;
            if (OK != alocCopy(third, &pt->word)) {
                printf("Out of memory!\n");
                return NOK;
            }
        }
    } else {
        if (pr) {
            pr->next = (RecordLL *) malloc(sizeof(RecordLL));
            pr = pr->next;
        } else {
            root = (RecordLL *) malloc(sizeof(RecordLL));
            pr = root;
        }
        if (!pr) {
            printf("Out of memory!\n");
            return NOK;
        }
        pr->next = 0;
        if (OK != alocCopy(first, &pr->record.first)
                || OK != alocCopy(second, &pr->record.second)) {
            return NOK;
        }
        pr->record.follows = (ThirdLL *) malloc(sizeof(ThirdLL));
        if (!pr->record.follows) {
            printf("Out of memory!\n");
            return NOK;
        }
        ++pr->record.count;
        pr->record.follows->next = 0;
        pr->record.follows->count = 1;
        if (OK != alocCopy(third, &pr->record.follows->word)) {
            return NOK;
        }
        ++allCount;
    }
    
    return OK;
}

int alocCopy(Txt source, Txt * dest) {
    int len = strlen(source);
    
    *dest = malloc(len + 1);
    if (!*dest) {
        printf("Out of memory!\n");
        return NOK;
    }
    strcpy(*dest, source);
    return OK;
}

RecordLL * newRecord(Txt first, Txt second, Txt third, int count, int thirdCount) {
    RecordLL * r;
    if (root) {
        last->next = (RecordLL *) malloc(sizeof(RecordLL));
        last = last->next;
        r = last;
    } else {
        root = (RecordLL *) malloc(sizeof(RecordLL));
        last = root;
        r = root;
    }
    if (!r) {
        printf("Out of memory!\n");
        return NULL;
    }
    r->next = 0;
    if (OK != alocCopy(first, &r->record.first)
            || OK != alocCopy(second, &r->record.second)) {
        printf("Out of memory!\n");
        return NULL;
    }
    r->record.count = count;
    r->record.follows = (ThirdLL *) malloc(sizeof(ThirdLL));
    if (!r->record.follows) {
        printf("Out of memory!\n");
        return NULL;
    }
    r->record.follows->count = thirdCount;
    if (OK != alocCopy(third, &r->record.follows->word)) {
        printf("Out of memory!\n");
        return NULL;
    }
    
    return r;
}

RecordLL * getRecord(int index) {
    int i;
    RecordLL * r = root;
    
    for (i = 0; i < index && r; ++i) {
        r = r->next;
    }
    return r;
}

Txt chooseThird(ThirdLL * words) {
    int points = 0;
    int draw;
    ThirdLL * t;
    
    for (t = words; t; t = t->next) {
        points += t->count;
    }
    draw = rand() % points;
    points = 0;
    
    for (t = words; t; t = t->next) {
        points += t->count;
        if (points > draw) {
            return t->word;
        }
    }
    return words->word;
}

RecordLL * markovNext(Txt first, Txt second) {
    RecordLL * r;
    
    for (r = root; r; r = r->next) {
        if (0 == strcmp(first, r->record.first)
                && 0 == strcmp(second, r->record.second)) {
            return r;
        }
    }
    return 0;
}

int main(int argc, char ** argv) {
    parseArgs(argc, argv);
    if (MODE_INCORRECT == options.mode) {
        usage(argv[0]);
        return 1;
    }
    if (MODE_DATABASE & options.mode) {
        if (OK != readDb(options.database)) {
            if (!(MODE_LEARN & options.mode)) {
                printf("Failed to read database from file %s.\n", options.database);
                return 1;
            }
        }
    }
    if (MODE_LEARN & options.mode) {
        if (OK != learn(options.input)) {
            printf("Failed to learn anything from file %s.\n", options.input);
            return 1;
        }
        if (MODE_DATABASE & options.mode) {
            if (OK != writeDb(options.database)) {
                printf("Failed to write database to file %s.\n", options.database);
                return 1;
            }
        }
    }
    if (MODE_WRITE & options.mode) {
        write();
    }
    freeDb();
    return 0;
}

