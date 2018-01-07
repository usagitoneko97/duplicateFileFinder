#ifndef _json2AvlOnFolder_H
#define _json2AvlOnFolder_H

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

void json2AvlOnFolder(JsonNode **root, char *path);
JsonNode *createNodeWithJson(json_t *json, char *name);
int avlCompareFp(void *data1, Node *data2);

#endif // _json2AvlOnFolder_H
