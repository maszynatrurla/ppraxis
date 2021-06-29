/**
 * @file columnar.c
 * @author maszynatrurla
 * @brief Programming exercise: Double transposition cipher.
 * 
 * Inspiration: http://programmingpraxis.com/2009/05/29/double-transposition-cipher/
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int * createTranspVector(char * pwd, int pwdlen) {
    int * vec = malloc(sizeof(int) * pwdlen);
    int i;
    
    if (!vec) {
        printf("Out of memory\n");
        return 0;
    }
    
    for (i = 0; i < pwdlen; ++i) {
        int j;
        unsigned char * min = (unsigned char *) pwd;
        
        for (j = 0; j < pwdlen; ++j) {
            if (*min > (unsigned char) pwd[j]) {
                min = (unsigned char *) &pwd[j];
            }
        }
        vec[i] = min - (unsigned char *) pwd;
        *min = 0xFF;
    }
    
    return vec;
}

void encrypt(char * txt, char * out, int len, int * vec, int vlen) {
    int i;
    int oi = 0;
    
    for (i = 0; i < vlen; ++i) {
        int j;
        
        for (j = vec[i]; j < len; j += vlen) {
            out[oi++] = txt[j];
        }
    }
    out[oi] = 0;
}

void decrypt(char * txt, char * out, int len, int * vec, int vlen) {
    int i;
    int ii = 0;
    
    for (i = 0; i < vlen; ++i) {
        int j;
        
        for (j = vec[i]; j < len; j += vlen) {
            out[j] = txt[ii++];
        }
    }
    out[len] = 0;
}


int main(int argc, char ** argv) {
    char cbuf [BUFSIZ];
    char obuf [BUFSIZ];
    void (*pFunModify)(char * txt, char * out, int len, int * vec, int vlen);
    int * v0 = 0;
    int * v1 = 0;
    int v0l;
    int v1l;
    
    if (argc != 4) {
        printf("Usage:\n%s encrypt|decrypt PWD1 PWD2\n", argv[0]);
        return 1;
    }
    if (0 == strcmp(argv[1], "encrypt")) {
        pFunModify = encrypt;
    } else if (0 == strcmp(argv[1], "decrypt")) {
        pFunModify = decrypt;
    } else {
        printf("Incorrect command: \"%s\". \nExpected: encrypt, decrypt.\n",
               argv[1]);
        return 1;
    }
    
    v0l = strlen(argv[2]);
    v1l = strlen(argv[3]);
    if (0 == (v0 = createTranspVector(argv[2], v0l))
            || 0 == (v1 = createTranspVector(argv[3], v1l))) {
        return 1;
    }
        
    while (0 != fgets(cbuf, sizeof(cbuf), stdin)) {
        int tl = strlen(cbuf) - 1;
        
        pFunModify(cbuf, obuf, tl, v0, v0l);
        pFunModify(obuf, cbuf, tl, v1, v1l);
        printf("%s\n", cbuf);
    }
    
    free(v0);
    free(v1);

    return 0;
}
