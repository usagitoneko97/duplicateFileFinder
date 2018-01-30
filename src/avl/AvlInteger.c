#include "AvlInteger.h"
#include "stdlib.h"
#include "malloc.h"
#include "string.h"


int compareIntwithVoidPtr(void *data1, Node *data2)
{
    int data2Int = (int)(data2->data);
    int *data1int = (int*)data1;
    IntegerNode *intNode2 = (IntegerNode *)data2;
    if (*data1int > data2Int)
    {
        return 1;
    }
    else if (*data1int < data2Int)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

int avlAddInteger(IntegerNode **root, IntegerNode *nodeToAdd){
    int data = nodeToAdd->data;
    Node *_nodeToAdd = (Node *)nodeToAdd;
    
    _nodeToAdd->data = (void*)malloc(sizeof(int));
    *(int*)(_nodeToAdd->data) = data;
    
    return _avlAdd((Node **)root, _nodeToAdd, compareIntwithVoidPtr);
}

Node *avlRemoveIntegerWithValue(IntegerNode **root, int data){
    return avlRemove((Node **)root, (void *)&data, compareIntwithVoidPtr);
}