#ifndef _NODE_H
#define _NODE_H

#include <stdint.h>
typedef struct Node Node;
struct Node{
  Node *left;
  Node *right;
  int bf;
  int data;
};

typedef struct StrNode StrNode;
struct StrNode{
  StrNode *left;
  StrNode *right;
  char* data;
};

#define CHANGED 1
#define NO_CHANGED 0

int compare(void *data, Node *node);
#endif // _NODE_H
