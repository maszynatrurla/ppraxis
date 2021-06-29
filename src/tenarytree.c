/**
 * @file tenarytree.c
 * @author maszynatrurla
 * @brief Programming exercise: Tenary search tree
 * 
 * Inspiration: http://programmingpraxis.com/2009/06/05/ternary-search-tries/
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OK      0
#define NOK     1

typedef void * Value;
typedef const char * Key;

typedef struct Node_t {
    struct Node_t * lt;
    struct Node_t * gt;
    struct Node_t * eq;
    char            key;
    Value           value;
} Node;

typedef struct {
    Key   key;
    Value value;
} Pair;

typedef struct {
    Node * root;
    size_t valueSize;
} MyMap;


void   mapInit(MyMap * map, size_t valueSize);
void   mapDelete(MyMap * map);
Value  mapGet(MyMap * map, Key key);
int    mapInsert(MyMap * map, Key key, Value value);
void   mapRemove(MyMap * map, Key key);
Pair * mapEnlist(MyMap * map, int * outSize);

static void  mapInternalDelete(Node * node);
static Value mapInternalGet(Node * node, Key key);
static int   mapInternalInsert(Node ** pNode, Key key, Value value);
static void  mapInternalRemove(Node * node, Key key);
static void  mapInternalEnlist(Node * node, Pair ** plist, int * size,
        char * key, const char * keyBegin);


void mapInit(MyMap * map, size_t valueSize) {
    map->root = 0;
    map->valueSize = valueSize;
}

void mapDelete(MyMap * map) {
    mapInternalDelete(map->root);
}

Value mapGet(MyMap * map, Key key) {
    if (!*key) {
        return 0;
    }
    return mapInternalGet(map->root, key);
}

int mapInsert(MyMap * map, Key key, Value value) {
    Value copy = malloc(map->valueSize);
    if (!copy) {
        printf("Out of memory\n");
        return NOK;
    }
    memcpy(copy, value, map->valueSize);
    return mapInternalInsert(&map->root, key, copy);
}

void mapRemove(MyMap * map, Key key) {
    if (*key) {
        mapInternalRemove(map->root, key);
    }
}

Pair * mapEnlist(MyMap * map, int * outSize) {
    Pair * list;
    char cbuf [BUFSIZ];
    
    *outSize = 0;
    if (!map->root) {
        return 0;
    }
    mapInternalEnlist(map->root, &list, outSize, cbuf, cbuf);
    return list;
}

static void mapInternalDelete(Node * node) {
    if (node->value) free(node->value);
    if (node->lt)    mapInternalDelete(node->lt);
    if (node->gt)    mapInternalDelete(node->gt);
    if (node->eq)    mapInternalDelete(node->eq);
    free(node);
}


static Value mapInternalGet(Node * node, Key key) {
    if (!node) {
        return 0;
    }
    if (*key < node->key) {
        return mapInternalGet(node->lt, key);
    }
    if (*key > node->key) {
        return mapInternalGet(node->gt, key);
    }
    if (*(++key)) {
        return mapInternalGet(node->eq, key);
    }
    return node->value;
}

static int mapInternalInsert(Node ** pNode, Key key, Value value) {
    Node * node = *pNode;
    
    if (!node) {
        node = malloc(sizeof(Node));
        if (!node) {
            printf("Out of memory\n");
            return NOK;
        }
        node->eq = 0;
        node->lt = 0;
        node->gt = 0;
        node->value = 0;
        node->key = *key;
        *pNode = node;
    }
    if (*key < node->key) {
        return mapInternalInsert(&node->lt, key, value);
    }
    if (*key > node->key) {
        return mapInternalInsert(&node->gt, key, value);
    }
    if (*(++key)) {
        return mapInternalInsert(&node->eq, key, value);
    }
    node->value = value;
    return OK;    
}

static void  mapInternalRemove(Node * node, Key key) {
    if (!node) {
        return;
    }
    if (*key < node->key) {
        mapInternalRemove(node->lt, key);
    } else if (*key > node->key) {
        mapInternalRemove(node->gt, key);
    } else if (*(++key)) {
        mapInternalRemove(node->eq, key);
    } else if (node->value) {
        free(node->value);
        node->value = 0;
    }
}

static void  mapInternalEnlist(Node * node, Pair ** plist, int * size,
        char * key, const char * keyBegin) {
    if (node->value) {
        Pair * nlist;
        if (*size) {
            nlist = realloc(*plist, (*size + 1) * sizeof(Pair));
        } else {
            nlist = malloc(sizeof(Pair));
        }
        if (!nlist) {
            printf("Out of memory\n");
            return;
        }
        nlist[*size].key = malloc(key - keyBegin + 2);
        if (!nlist[*size].key) {
            printf("Out of memory\n");
            return;
        }
        *(key++) = node->key;
        memcpy((void *) (nlist[*size].key), keyBegin, key - keyBegin);
        ((char *) nlist[*size].key)[key - keyBegin] = 0;
        nlist[*size].value = node->value;
        ++(*size);
        *plist = nlist;
    }
    if (node->lt) {
        mapInternalEnlist(node->lt, plist, size, key, keyBegin);
    }
    if (node->eq) {
        if (!node->value) {
            *(key++) = node->key;
        }
        mapInternalEnlist(node->eq, plist, size, key, keyBegin);
    }
    if (node->gt) {
        if (node->eq && !node->value) {
            --key;
        }
        mapInternalEnlist(node->gt, plist, size, key, keyBegin);
    }
}

int main(int argc, char ** argv) {
    MyMap map;
    FILE * fp;
    char cbuf[BUFSIZ];
    Pair * list;
    int size;
    int i;
    Value val;
    
    if (argc > 2) {
        printf("Usage:\n%s [FILENAME]\n", argv[0]);
        return 1;
    }
    if (argc == 2) {
        fp = fopen(argv[1], "r");
        if (!fp) {
            printf("Failed to open file \"%s\" for reading\n", argv[1]);
            return 1;
        }
    } else {
        fp = stdin;
    }
    
    mapInit(&map, sizeof(int));
    
    while (1 == fscanf(fp, "%s", cbuf)) {
        Value v = mapGet(&map, cbuf);
        if (v) {
            ++(*((int *) v));
        } else {
            int value = 1;
            if (OK != mapInsert(&map, cbuf, &value)) {
                printf("Map insert failed\n");
                break;
            }
        }
    }
    
    fclose(fp);
    
    val = mapGet(&map, "and");
    if (val) {
        printf("Word \"and\": %d\n", *((int *) val));
        mapRemove(&map, "and");
    }
    
    list = mapEnlist(&map, &size);
    
    for (i = 0; i < size; ++i) {
        printf("%-32s  : %d\n", list[i].key, *((int *) list[i].value));
    }
    
     mapDelete(&map);
    
    return 0;
}
