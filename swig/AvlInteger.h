#ifndef _AVLINTEGER_H
#define _AVLINTEGER_H

#include "Node.h"
#include "AddAvl.h"
#include "DeleteAvl.h"
typedef struct IntegerNode IntegerNode;
struct IntegerNode
{
    IntegerNode *left;
    IntegerNode *right;
    int balanceFactor;
    int data;
};

// #define avlRemoveIntegerWithValue(root, data) avlRemove((Node **)root, (void*)&data, compareIntwithVoidPtr)
Node *avlRemoveIntegerWithValue(IntegerNode **root, int data);
int avlAddInteger(IntegerNode **root, IntegerNode *nodeToAdd);
int compareInt(Node *node1, Node *node2);
int compareIntwithVoidPtr(void *data1, Node *data2);
#endif // _AVLINTEGER_H
