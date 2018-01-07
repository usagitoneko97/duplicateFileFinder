#ifndef _JSON2AVL_H
#define _JSON2AVL_H

#include "Node.h"
typedef struct jsonNode jsonNode;
struct jsonNode
{
    jsonNode *left;
    jsonNode *right;
    int balanceFactor;
    char *data;
};

#endif // _JSON2AVL_H
