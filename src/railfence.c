/**
 * @file railfence.c
 * @author maszynatrurla
 * @brief Programming exercise: Rail Fence cipher.
 * 
 * Inspiration: http://programmingpraxis.com/2009/03/31/rail-fence-cipher/
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OK      0
#define NOK     1

#define KEY     4

static void encrypt(const char * file);
static void decrypt(const char * file);
static int  read   (const char * file);

static char * txt;
static int len;

void encrypt(const char * file) {
    int i;
    
    if (OK != read(file)) {
        return;
    }
    
    for (i = 0; i < KEY; ++i) {
        int skokA = 2 * (KEY - i) - 2;
        int skokB = 2 * i;
        int j = i;
        
        putchar(txt[j]);
        for (; j < len;) {
            j += skokA;
            if (skokA && j < len) {
                putchar(txt[j]);
            }
            j += skokB;
            if (skokB && j < len) {
                putchar(txt[j]);
            }
        }
    }
    
    putchar('\n');
}

void decrypt(const char * file) {
    FILE * fp;
    int l = 0;
    int res = OK;
    
    fp = fopen(file, "r");
    if (!fp) {
        printf("Failed to read file: %s\n", file);
        res = NOK;
        
    }
    if (OK == res && OK != fseek(fp, 0, SEEK_END)) {
        printf("Failed to read file: %s\n", file);
        res = NOK;
    }
    if (OK == res) {
        l = ftell(fp);
        txt = malloc(l);
        if (!txt) {
            printf("Out of memory!\n");
            res = NOK;
        }
    }
    if (OK == res) {
        int i;
        
        fseek(fp, 0, SEEK_SET);
        for (i = 0; i < KEY; ++i) {
            int skokA = 2 * (KEY - i) - 2;
            int skokB = 2 * i;
            int j = i;
            char ch;
            
            if (EOF == (ch = fgetc(fp))) {
                break;
            }
            txt[j] = ch;
            for (;j < l;) {
                j += skokA;
                if (skokA && j < l) {
                    if (EOF == (ch = fgetc(fp))) {
                        break;
                    }
                    txt[j] = ch;
                }
                j += skokB;
                if (skokB && j < l) {
                    if (EOF == (ch = fgetc(fp))) {
                        break;
                    }
                    txt[j] = ch;
                }
            }
        }
        
        for (i = 0; i < l; ++i) {
            putchar(txt[i]);
        }
    }
    
    fclose(fp);
    putchar('\n');
}

int read(const char * file) {
    FILE * fp;
    int res = OK;
    int l = 0;
    
    fp = fopen(file, "r");
    if (!fp) {
        printf("Failed to read file: %s\n", file);
        res = NOK;
        
    }
    if (OK == res && OK != fseek(fp, 0, SEEK_END)) {
        printf("Failed to read file: %s\n", file);
        res = NOK;
    }
    if (OK == res) {
        l = ftell(fp);
        txt = malloc(l);
        if (!txt) {
            printf("Out of memory!\n");
            res = NOK;
        }
    }
    if (OK == res) {
        fseek(fp, 0, SEEK_SET);
        if (l != fread(txt, 1, l, fp)) {
            res = NOK;
        }
    }
    len = l;
    
    fclose(fp);
    return res;
}

int main(int argc, char ** argv) {
    if (argc != 3) {
        printf("Usage:\n%s (encrypt|decrypt) FILE\n", argv[0]);
        return NOK;
    }
    if (0 == strcmp("encrypt", argv[1])) {
        encrypt(argv[2]);
    } else if (0 == strcmp("decrypt", argv[1])) {
        decrypt(argv[2]);
    } else {
        printf("Unknown command: %s\n", argv[1]);
        return NOK;
    }
    return 0;
}
