/**
 * @file quine.c
 * @author maszynatrurla
 * @brief Programming exercise: Self-replicating code.
 * 
 * Inspiration: http://programmingpraxis.com/2009/02/20/a-self-reproducing-program/
 */

#include <stdio.h>

#define PROLOG  14
#define ALL     40

const char * lines [] = {
    "/**",
    " * @file quine.c",
    " * @author maszynatrurla",
    " * @brief Programming exercise: Self-replicating code.",
    " * ",
    " * Inspiration: http://programmingpraxis.com/2009/02/20/a-self-reproducing-program/",
    " */",
    "",
    "#include <stdio.h>",
    "",
    "#define PROLOG  14",
    "#define ALL     40",
    "",
    "const char * lines [] = {",
    "};",
    "",
    "int main(int argc, char ** argv) {",
    "    int i;",
    "    for (i = 0; i < PROLOG; ++i) {",
    "        printf(lines[i]);",
    "        putchar(10);",
    "    }",
    "    for (i = 0; i < ALL; ++i) {",
    "        putchar(0x20);",
    "        putchar(0x20);",
    "        putchar(0x20);",
    "        putchar(0x20);",
    "        putchar(34);",
    "        printf(lines[i]);",
    "        putchar(34);",
    "        putchar(44);",
    "        putchar(10);",
    "    }",
    "    for (i = PROLOG; i < ALL - 1; ++i) {",
    "        printf(lines[i]);",
    "        putchar(10);",
    "    }",
    "    return 0;",
    "}",
    "",
};

int main(int argc, char ** argv) {
    int i;
    for (i = 0; i < PROLOG; ++i) {
        printf(lines[i]);
        putchar(10);
    }
    for (i = 0; i < ALL; ++i) {
        putchar(0x20);
        putchar(0x20);
        putchar(0x20);
        putchar(0x20);
        putchar(34);
        printf(lines[i]);
        putchar(34);
        putchar(44);
        putchar(10);
    }
    for (i = PROLOG; i < ALL - 1; ++i) {
        printf(lines[i]);
        putchar(10);
    }
    return 0;
}
