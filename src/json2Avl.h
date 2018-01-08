#ifndef _json2AvlOnFolder_H
#define _json2AvlOnFolder_H

#include "fileHandler.h"
#include "Node.h"
#include "linkedlist.h"

typedef struct JsonNode JsonNode;
struct JsonNode
{
    JsonNode *left;
    JsonNode *right;
    int balanceFactor;
    FileProperty *data;
};

typedef struct DuplicationList DuplicationList;
struct DuplicationList{
    int numberOfDuplication;
    LinkedList *list;
};

void json2Avl(JsonNode **root, char *path, DuplicationList *duplicateL);
void json2AvlOnFolder(JsonNode **root, char *path, DuplicationList *duplicateL);
JsonNode *createNodeWithJson(json_t *json, char *name);
int avlCompareFp(void *data1, Node *data2);
int avlCompareFpWithCrc(void *data1, Node *data2);
Item *createItemWithNode(JsonNode *node);
int searchCrcOnList(LinkedList *list, int numberOfList, int crc);
void freeJsonNode(JsonNode *jsonRoot);
void freeDuplicationLinkedList(DuplicationList *dupList);
void freeItem(Item *item);

#define avlAddJsonFp(jsonRootNode, nodeToAdd, avlCompareFp) \
    _avlAdd((Node **)jsonRootNode, (Node *)nodeToAdd, avlCompareFp)

#endif // _json2AvlOnFolder_H
