#ifndef _NODE_H
#define _NODE_H

#include <stdint.h>



typedef struct Node Node;
struct Node{
  Node *left;
  Node *right;
  int balanceFactor;
  void *data;
};

typedef int (*Compare)(Node *, Node *);
typedef int (*CompareWithVoidPtr)(void *, Node *);
#define CHANGED 1
#define NO_CHANGED 0



#endif // _NODE_H
