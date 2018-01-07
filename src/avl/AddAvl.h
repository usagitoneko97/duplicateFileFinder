#ifndef _avlAdd_H
#define _avlAdd_H

#include "Node.h"

int _avlAdd(Node **root, Node *nodeToAdd, CompareWithVoidPtr compare);
void avlAdd(Node **root, Node *NodeToAdd, CompareWithVoidPtr compare);
int avlBalanceLeftTree(Node **rootPtr);
int avlBalanceRightTree(Node **rootPtr);




#endif // _avlAdd_H
