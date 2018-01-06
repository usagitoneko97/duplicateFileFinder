#include "AddAvl.h"
#include "rotate.h"

int heightChanged;

/** 
 * @brief  add a value to the avl tree
 * @param  **rootPtr: pointer to the avl tree
 * @param  *nodeToAdd: node structure to add
 * @retval height change status of the avl tree after addition
 */
int addAvl(Node **rootPtr, Node *nodeToAdd){
  if(*(rootPtr) == NULL){
    nodeToAdd->left = NULL;
    nodeToAdd->right = NULL;
    nodeToAdd->bf = 0;
    *rootPtr = nodeToAdd;
    return CHANGED;
  }
  else{
    if(nodeToAdd->data > (*(rootPtr))->data){
      heightChanged = addAvl(&(*(rootPtr))->right, nodeToAdd);
      // (*rootPtr)->right = (*rootPtr);
      //calc bf
      // (*rootPtr)->bf ++;
      if(heightChanged == CHANGED){
        (*rootPtr)->bf ++;
        if((*rootPtr)->bf == 0){
          return NO_CHANGED;
        }
      }
      else {
        return NO_CHANGED;
      }
      //  (*rootPtr)->bf = calcBF(*rootPtr);
      if ((*rootPtr)->bf != 2)
      {
        return CHANGED;
      }
      //change in structure of tree (decreasing level), will results in no change in the bf
      if(avlBalanceRightTree(rootPtr) == CHANGED){
        return NO_CHANGED;
      }
      else{
        return CHANGED;
      }
    }
    else{
      heightChanged = addAvl(&(*(rootPtr))->left, nodeToAdd);
      if(heightChanged == CHANGED){
        (*rootPtr)->bf --;
        if ((*rootPtr)->bf == 0)
        {
          return NO_CHANGED;
        }
      }
      else  
        return NO_CHANGED;
      if((*rootPtr)->bf != -2){
        return CHANGED;
      }
      //change in structure of tree (decreasing level), will results in no change in the bf
      if (avlBalanceLeftTree(rootPtr) == CHANGED)
      {
        return NO_CHANGED;
      }
      else
      {
        return CHANGED;
      }
    }
    // return (*rootPtr);
  }
}


