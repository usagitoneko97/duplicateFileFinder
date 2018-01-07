#ifndef _JSON2AVL_H
#define _JSON2AVL_H

#include "fileHandler.h"

#include "Node.h"
typedef struct JsonNode JsonNode;
struct JsonNode
{
    JsonNode *left;
    JsonNode *right;
    int balanceFactor;
    FileProperty *data;
};

void json2Avl(JsonNode **root, char *path);
JsonNode *createNodeWithJson(json_t *json, char *name);
int avlCompareFp(void *data1, Node *data2);

#endif // _JSON2AVL_H
