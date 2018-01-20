#include "AvlSearch.h"

Node *avlSearch(Node *nodePtr, void *key, CompareWithVoidPtr compare){
    if(nodePtr == NULL){
        return NULL;
    }
    if(compare(key, nodePtr) == 1){
        return avlSearch(nodePtr->right, key, compare);
    }
    else if (compare(key, nodePtr) == -1){
        return avlSearch(nodePtr->left, key, compare);
    }
    else{
        return nodePtr;
    }
}