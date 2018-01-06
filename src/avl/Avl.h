#ifndef _AVL_H
#define _AVL_H

#include "Node.h"

int addAvl(Node **root, Node *nodeToAdd);
int avlBalanceLeftTree(Node **rootPtr);
int avlBalanceRightTree(Node **rootPtr);

Node *avlDeleteNearestLeft(Node **rootPtr, int *heightChangedStatus);
Node *avlDelete(Node **rootPtr, int data);
Node *_avlDelete(Node **rootPtr, void *data, int *heightChangedStatus);
Node *avlFindMin(Node *root);
Node *avlFindMax(Node *root);

Node *rotateLeft(Node *root);
Node *rotateRight(Node *root);
Node *rotateleftRight(Node *node);
Node *rotateRightLeft(Node *node);
int calcBF(Node *root);
int nodeHeight(Node *root);
int avlBalanceLeftTree(Node **rootPtr);
int avlBalanceRightTree(Node **rootPtr);

#endif // _AVL_H
