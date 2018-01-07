#ifndef _AVL_H
#define _AVL_H

#include "Node.h"


int _avlAdd(Node **root, Node *nodeToAdd);
void avlAdd(Node **root, Node *nodeToAdd);
int avlBalanceLeftTree(Node **rootPtr);
int avlBalanceRightTree(Node **rootPtr);

Node *avlRemoveNearestLeft(Node **rootPtr, int *heightChangedStatus);
Node *avlRemove(Node **rootPtr, int data);
Node *_avlRemove(Node **rootPtr, void *data, int *heightChangedStatus);
Node *avlFindMin(Node *root);
Node *avlFindMax(Node *root);

Node *rotateLeft(Node *root);
Node *rotateRight(Node *root);
Node *rotateleftRight(Node *node);
Node *rotateRightLeft(Node *node);

int calcbalanceFactor(Node *root);
int nodeHeight(Node *root);
int avlBalanceLeftTree(Node **rootPtr);
int avlBalanceRightTree(Node **rootPtr);

#endif // _AVL_H
