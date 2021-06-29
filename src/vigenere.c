/**
 * @file vigenere.c
 * @author maszynatrurla
 * @brief Programming exercise: XOR password cypher.
 * 
 * Inspiration: http://programmingpraxis.com/2009/03/03/creation/
 */

#include <stdio.h>
#include <stdlib.h>

#define FLEN                    5
#define PRINTABLE_FROM          32
#define PRINTABLE_UNTIL         126

static void read(const char * file, int really);
static void write(int dec);
static void findPassLen(void);
static int findRepetitions(int n, int * reps);
static void findPwd(int keyLen);

static char * code;
static int    codeLen = 0;
static char   pwdable [] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q',
    'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

void read(const char * file, int really) {
    FILE * fp;
    int len;
    
    fp = fopen(file, "r");
    if (!fp) {
        printf("Could not read file \"%s\"\n", file);
        return;
    }
    for (len = 0; !feof(fp) && !ferror(fp);) {
        int dummy;
        len += fscanf(fp, "%d", &dummy);
        if (really) {
            code[len] = (char) dummy;
        }
    }
    fclose(fp);
    
    if (!really) {
        codeLen = len;
        code = (char *) malloc(codeLen * sizeof(char));
        read(file, 1);
    }
}

void write(int dec) {
    int i;
    char pwd [] = "sGenesi";
    
    for (i = 0; i < codeLen; ++i) {
        if (dec) {
            printf("%d ", code[i]);
            if (0 == i % 30) printf("\n");
        } else {
            putchar(code[i] ^ pwd[i % 7]);
        }
    }
    printf("\n");
}

void findPassLen() {
    int i;
    int chosen[128];
    int chosenLen = 0;
    
    for (i = 0; i < codeLen - FLEN; ++i) {
        int repetitions[512];
        int reps = findRepetitions(i, repetitions);
        if (reps > 8) {
            int j;
            int ok = 0;
            
            for (j = 0; j < chosenLen; ++j) {
                int k;
                
                for (k = 0; k < FLEN; ++k) {
                    if (code[i + k] != code[chosen[j] + k]) {
                        ++ok;
                        break;
                    }
                }
            }
            if (ok == chosenLen) {
                int k;
                
                chosen[chosenLen++] = i;
                
                repetitions[0] = i;
                for (k = 1; k < reps; ++k) {
                    printf("%d ", repetitions[k] - repetitions[k - 1]);
                }
                printf("\n");
                
                
//                 printf("\n%d %d %d %d %d: %d\n\t", code[i], code[i+1], code[i+2], code[i+3], code[i+4], reps);
//                 
//                 for (j = 0; j < reps; ++j) {
//                     printf("%d ", repetitions[j]);
//                 }
            }
        }
    }
}

void findPwd(int keyLen) {
    int i;
    
    for (i = 0; i < keyLen; ++i) {
        int pi;
        
        for (pi = 0; pi < sizeof(pwdable); ++pi) {
            int j;
            int ook = 1;
            
            for (j = i; j < codeLen; j += keyLen) {
                char dec = code[j] ^ pwdable[pi];
                if (dec != 0xA /*&& dec != 0xD*/  && (dec < PRINTABLE_FROM || dec > PRINTABLE_UNTIL)) {
                    ook = 0;
                    break;
                }
            }
            if (ook) {
                printf("%c ", pwdable[pi]);
            }
        }
        printf("\n");
        
    }
}

void findA() {
    char p0 [] = {'s'/*, 't'*/};
    char p1 [] = {'G'};
    char p2 [] = {'a', 'b', /*'c',*/ 'd', 'e', 'f', 'g', 'k', 'p', 'z'};
    char p3 [] = {'i', 'j', 'k', 'l', 'm', 'n', 'o', 'q'};
    char p4 [] = {/*'b',*/ 'e'};
    char p5 [] = {'s'};
    char p6 [] = {'i'};
    int i0, i1, i2, i3, i4, i5, i6;
    
    for (i0 = 0; i0 < sizeof(p0); ++i0) {
        for (i1 = 0; i1 < sizeof(p1); ++i1) {
            for (i2 = 0; i2 < sizeof(p2); ++i2) {
                for (i3 = 0; i3 < sizeof(p3); ++i3) {
                    for (i4 = 0; i4 < sizeof(p4); ++i4) {
                        for (i5 = 0; i5 < sizeof(p5); ++i5) {
                            for (i6 = 0; i6 < sizeof(p6); ++i6) {
                                int i;
                                printf("%c%c%c%c%c%c%c: ",
                                       p0[i0], p1[i1], p2[i2], p3[i3], p4[i4], p5[i5], p6[i6]);
                                for (i = 0; i < 64; i += 7) {
                                    putchar(code[i] ^ p0[i0]);
                                    putchar(code[i + 1] ^ p1[i1]);
                                    putchar(code[i + 2] ^ p2[i2]);
                                    putchar(code[i + 3] ^ p3[i3]);
                                    putchar(code[i + 4] ^ p4[i4]);
                                    putchar(code[i + 5] ^ p5[i5]);
                                    putchar(code[i + 6] ^ p6[i6]);
                                }
                                printf("\n");
                            }
                        }
                    }
                }
            }
        }
        
    }
    
}

int findRepetitions(int n, int * reps) {
    int r = 0;
    int i;
    
    for (i = n + FLEN; i < codeLen - FLEN; ++i) {
        int j;
        int ok = 1;
        
        for (j = 0; j < FLEN; ++j) {
            if (code[n + j] != code[i + j]) {
                ok = 0;
                break;
            }
        }
        if (j == FLEN && ok) {
            reps[++r] = i;
        }
    }
    
    return r;
}


int main(int argc, char ** argv) {
    if (argc != 2) {
        printf("Usage: %s FILENAME\n", argv[0]);
        return 1;
    }
    
    read(argv[1], 0);
    findPassLen();
    findPwd(7);
    findA();
    write(0);
    
    return 0;
}



