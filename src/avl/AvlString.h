#ifndef _AVLSTRING_H
#define _AVLSTRING_H

#include "Node.h"
#include "AddAvl.h"
typedef struct StrNode StrNode;
struct StrNode
{
    StrNode *left;
    StrNode *right;
    int balanceFactor;
    char *data;
};

#define avlAddString(root, nodeToAdd)                   _avlAdd((Node **)root, (Node *)nodeToAdd, compareStrVoidPtr)
#define avlRemoveStringWithValue(root, strToDelete)    avlRemove((Node **)root, (void*)strToDelete, compareStrVoidPtr)

int compareStrVoidPtr(void *str1, Node *str2);
#endif // _AVLSTRING_H
