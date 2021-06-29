/**
 * @file piglatin.c
 * @author maszynatrurla
 * @brief Programming exercise: Translate English text into pig latin
 *
 * Inspiration: http://programmingpraxis.com/2009/06/02/pig-latin/
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

const char vovels [] = "aeiouy";

int isVovel(char a) {
    int i;
    char A = tolower(a);

    for (i = 0; vovels[i]; ++i) {
        if (A == vovels[i]) {
            return 1;
        }
    }
    return 0;
}

void usage(const char * pgm) {
    printf("Usage:\n%s encode|decode\n", pgm);
}

char * encode(char * inp) {
    static char out[BUFSIZ + 5];
    if (isVovel(inp[0])) {
        strcpy(out, inp);
        strcpy(out + strlen(inp), "-way");
    } else {
        int i;
        for (i = 0; inp[i]; ++i) {
            if (isVovel(inp[i])) {
                int len;
                strcpy(out, inp + i);
                len = strlen(out);
                out[len++] = '-';
                inp[i] = 0;
                strcpy(&out[len], inp);
                strcpy(out+len+i,"ay");
                inp[i] = 42;
                break;
            }

        }
        if (!inp[i]) {
            return inp;
        }
    }
    return out;
}

char * decode(char * inp) {
    int i, oi = 0;
    int pref;
    static char oubuf[BUFSIZ];

    for (i = 0; inp[i]; ++i) {
        if ('-' == inp[i]) {
            pref = i;
            break;
        }
    }
    if (!inp[i]) return inp;
    for (++i; inp[i]; ++i) {
        if (!isVovel(inp[i])) {
            oubuf[oi++] = inp[i];
        } else {
            break;
        }
    }
    for (i = 0; i < pref; ++i) {
        oubuf[oi++] = inp[i];
    }

    oubuf[oi] = 0;

    return oubuf;
}

int main(int argc, char ** argv) {
    int encodeMode;
    char cbuf[BUFSIZ];

    if (argc != 2) {
        usage(argv[0]);
        return 1;
    }
    if (0 == strcmp("encode", argv[1])) {
        encodeMode = 1;
    } else if (0 == strcmp("decode", argv[1])) {
        encodeMode = 0;
    } else {
        usage(argv[0]);
        return 1;
    }

    while (1 == scanf("%s", cbuf)) {
        printf("%s ", encodeMode ? encode(cbuf) : decode(cbuf));
    }

    return 0;
}
