/**
 * @file csv.c
 * @author maszynatrurla
 * @brief Programming exercise: Read Comma-Separated-Values.
 * 
 * Inspiration: http://programmingpraxis.com/2009/03/17/comma-separated-values/
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OK      0
#define NOK     1

typedef struct Cell_t {
    char * txt;
    struct Cell_t * next;
} Cell;

typedef struct Record_t {
    struct Cell_t * cells;
    struct Record_t * next;
} Record;

typedef enum {
    STARTCELL,
    CELL,
    QUOTE,
    ENDLINE,
    ENDQUOTE,
} State;

static int  readCSV(const char * file);
static void genHTML(void);

static int addCell(const char * txt);
static int addRecord();

static Record * root  = 0;
static Record * last  = 0;
static Cell   * cell  = 0;

int  readCSV(const char * file) {
    FILE * fp;
    State state = STARTCELL;
    char ch;
    char buf[BUFSIZ] = {0};
    int  bufi = 0;
    
    fp = fopen(file, "r");
    if (!fp) {
        printf("Could not read file %s\n", file);
        return NOK;
    }
    
    while (EOF != (ch = fgetc(fp))) {
        switch (state) {
            case STARTCELL:
                switch (ch) {
                    case ',':
                        if (OK != addCell(buf)) return NOK;
                        break;
                    case '\r':
                    case '\n':
                        buf[bufi % BUFSIZ] = 0;
                        if (OK != addCell(buf)) return NOK;
                        bufi = 0;
                        buf[0] = 0;
                        if (OK != addRecord()) return NOK;
                        state = ENDLINE;
                        break;
                    case '"':
                        state = QUOTE;
                        break;
                    default:
                        buf[bufi++ % BUFSIZ] = ch;
                        state = CELL;
                        break;
                }
                break;
            case CELL:
                switch (ch) {
                    case ',':
                        buf[bufi % BUFSIZ] = 0;
                        if (OK != addCell(buf)) return NOK;
                        bufi = 0;
                        buf[0] = 0;
                        state = STARTCELL;
                        break;
                    case '\r':
                    case '\n':
                        buf[bufi % BUFSIZ] = 0;
                        if (OK != addCell(buf)) return NOK;
                        bufi = 0;
                        buf[0] = 0;
                        if (OK != addRecord()) return NOK;
                        state = ENDLINE;
                        break;
                    default:
                        buf[bufi++ % BUFSIZ] = ch;
                        break;
                }
                break;
            case QUOTE:
                switch (ch) {
                    case '"':
                        state = ENDQUOTE;
                        break;
                    default:
                        buf[bufi++ % BUFSIZ] = ch;
                        break;
                }
                break;
            case ENDQUOTE:
                switch (ch) {
                    case '"':
                        buf[bufi++ % BUFSIZ] = '"';
                        state = QUOTE;
                        break;
                    case ',':
                        buf[bufi % BUFSIZ] = 0;
                        if (OK != addCell(buf)) return NOK;
                        bufi = 0;
                        buf[0] = 0;
                        state = STARTCELL;
                        break;
                    case '\r':
                    case '\n':
                        buf[bufi % BUFSIZ] = 0;
                        if (OK != addCell(buf)) return NOK;
                        bufi = 0;
                        buf[0] = 0;
                        if (OK != addRecord()) return NOK;
                        state = ENDLINE;
                        break;
                    default:
                        buf[bufi++ % BUFSIZ] = ch;
                        state = CELL;
                        break;
                }
                break;
            case ENDLINE:
                switch (ch) {
                    case '\r':
                    case '\n':
                        break;
                    case ',':
                        if (OK != addCell(buf)) return NOK;
                        break;
                    case '"':
                        state = QUOTE;
                        break;
                    default:
                        buf[bufi++ % BUFSIZ] = ch;
                        state = CELL;
                        break;
                }
                break;
            default:
                printf("Damn, wrong state\n");
                break;
        }
        
        
        if (feof(fp) || ferror(fp)) {
            break;
        }
    }
    if (buf[0]) {
        buf[bufi % BUFSIZ] = 0;
        if (OK != addCell(buf)) return NOK;
    }
    
    fclose(fp);
    
    return OK;
}

int addCell(const char * txt) {
    if (!last) {
        if (0 == txt[0]) {
            return OK;
        }
        root = malloc(sizeof(Record));
        if (!root) {
            printf("Out of memory!\n");
            return NOK;
        }
        root->next = 0;
        last = root;
        cell = 0;
    }
    if (!cell) {
        last->cells = malloc(sizeof(Cell));
        cell = last->cells;
    } else {
        cell->next = malloc(sizeof(Cell));
        cell = cell->next;
    }
    if (!cell) {
        printf("Out of memory!\n");
        return NOK;
    }
    cell->next = 0;
    cell->txt = malloc(strlen(txt));
    if (!cell->txt) {
        printf("Out of memory!\n");
        return NOK;
    }
    strcpy(cell->txt, txt);
    return OK; 
}

int addRecord() {
    if (!last || !cell) {
        return OK;
    }
    last->next = malloc(sizeof(Record));
    if (!last->next) {
        printf("Out of memory!\n");
        return NOK;
    }
    last = last->next;
    last->next = 0;
    cell = 0;
    return OK;
}

void genHTML(void) {
    Record * r;
    
    printf("<html><head><title>:P</title></head>\n<body>\n");
    printf("<table>\n");
    
    for (r = root; r; r = r->next) {
        Cell * c;
        
        printf("\t<tr>\n");
        for (c = r->cells; c; c = c->next) {
            int i;
            
            printf("\t\t<td style=\"border:solid\">");
            for (i = 0; c->txt[i]; ++i) {
                switch (c->txt[i]) {
                    case '<':
                        printf("&lt;");
                        break;
                    case '>':
                        printf("&gt;");
                        break;
                    case '"':
                        printf("&quot;");
                        break;
                    case '&':
                        printf("&amp;");
                        break;
                    default:
                        putchar(c->txt[i]);
                        break;
                }
            }
            printf("</td>\n");
        }
        printf("\t</tr>\n");
    }
    
    printf("</table></body></html>\n");
}

int main(int argc, char ** argv) {
    if (2 != argc) {
        printf("Usage: %s FILENAME\n", argv[0]);
        return 1;
    }
    
    if (OK != readCSV(argv[1])) {
        return 1;
    }
    genHTML();
    
    return 0;
}
