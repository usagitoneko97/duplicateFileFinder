#include "AddAvl.h"
#include "rotate.h"
#include "Exception.h"
#include "CException.h"
#include <stdio.h>
int heightChanged;

/** 
 * @brief  add a value to the avl tree
 * @param  **rootPtr: pointer to the avl tree
 * @param  *NodeToAdd: Node structure to add
 * @retval height change status of the avl tree after addition
 */
int _avlAdd(Node **rootPtr, Node *NodeToAdd, CompareWithVoidPtr compare)
{
  if(*(rootPtr) == NULL){
    NodeToAdd->left = NULL;
    NodeToAdd->right = NULL;
    NodeToAdd->balanceFactor = 0;
    *rootPtr = NodeToAdd;
    return CHANGED;
  }
  else{
    if(compare(NodeToAdd->data, *rootPtr) == 1)
    {
      heightChanged = _avlAdd(&(*(rootPtr))->right, NodeToAdd, compare);
      // (*rootPtr)->right = (*rootPtr);
      //calc balanceFactor
      // (*rootPtr)->balanceFactor ++;
      if(heightChanged == CHANGED){
        (*rootPtr)->balanceFactor ++;
        if((*rootPtr)->balanceFactor == 0){
          return NO_CHANGED;
        }
      }
      else {
        return NO_CHANGED;
      }
      //  (*rootPtr)->balanceFactor = calcbalanceFactor(*rootPtr);
      if ((*rootPtr)->balanceFactor != 2)
      {
        return CHANGED;
      }
      //change in structure of tree (decreasing level), will results in no change in the balanceFactor
      if(avlBalanceRightTree(rootPtr) == CHANGED){
        return NO_CHANGED;
      }
      else{
        return CHANGED;
      }
    }
    else if(compare(NodeToAdd->data, *rootPtr) == -1)
    {
      heightChanged = _avlAdd(&(*(rootPtr))->left, NodeToAdd, compare);
      if(heightChanged == CHANGED){
        (*rootPtr)->balanceFactor --;
        if ((*rootPtr)->balanceFactor == 0)
        {
          return NO_CHANGED;
        }
      }
      else  
        return NO_CHANGED;
      if((*rootPtr)->balanceFactor != -2){
        return CHANGED;
      }
      //change in structure of tree (decreasing level), will results in no change in the balanceFactor
      if (avlBalanceLeftTree(rootPtr) == CHANGED)
      {
        return NO_CHANGED;
      }
      else
      {
        return CHANGED;
      }
    }
    else{
      Throw(createException("Node to add is already exist in the avl tree", NODE_ADD_EXIST));
    }
  }
}

void avlAdd(Node **root, Node *NodeToAdd, CompareWithVoidPtr compare)
{
  _avlAdd(root, NodeToAdd, compare);
}
