#ifndef _ROTATE_H
#define _ROTATE_H
#include "Node.h"

Node *rotateLeft(Node *root);
Node *rotateRight(Node *root);
Node *rotateleftRight(Node *node);
Node *rotateRightLeft(Node *node);
int calcbalanceFactor(Node *root);
int nodeHeight(Node *root);
int avlBalanceLeftTree(Node **rootPtr);
int avlBalanceRightTree(Node **rootPtr);
#endif // _ROTATE_H
