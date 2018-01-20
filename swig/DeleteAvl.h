#ifndef _DELETEAVL_H
#define _DELETEAVL_H

#include "Node.h"
Node *avlRemoveNearestLeft(Node **rootPtr, int *heightChangedStatus);
Node *avlRemove(Node **rootPtr, void *data, CompareWithVoidPtr compare);
Node *_avlRemove(Node **rootPtr, void *data, int *heightChangedStatus, CompareWithVoidPtr compare);


#endif // _DELETEAVL_H
