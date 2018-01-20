#include "rotate.h"
#include <stddef.h>
/**
 *       25                      35
 *        \                     /  \
 *        35             ----> 25   40
 *       / \                    \
 *      30  40                   30
 */
/**
 * @brief  rotate left of the avl tree
 * @param  *node: avl tree
 * @retval return the new root after rotation
 */
Node *rotateLeft(Node *node){
  Node *root;
  root = node->right;
  if(root->left != NULL){
      node->right = node->right->left;
  }
  else{
    node->right = NULL;
  }
  root->left = node;
  return root;

}

/**
 *       30         10
 *      /          /  \
 *    10    ----> 5   30
 *   / \             /
 *  5  20          20
 */
/**
 * @brief  rotate right of the avl tree
 * @param  *node: avl tree
 * @retval return the new root after rotation
 */
Node *rotateRight(Node *node){
  Node *root;
  root = node->left;
  if(node->left->right != NULL){
    node->left = node->left->right;
  }
  else{
    node->left = NULL;
  }
    root->right = node;

  return root;
}

/**
 *
 *     30                      15
 *    /  \                    /  \
 *   10  50       ---->     10   30
 *  / \                    /    /  \
 * 5  15                  5    20   50
 *     \
 *     20
 */
Node *rotateleftRight(Node *node){
  Node *root, *temp;
  temp = rotateLeft(node->left);
  node->left = temp;
  root = rotateRight(node);
  return root;
}

//TODO test for this func
Node *rotateRightLeft(Node *node){
  Node *root, *temp;
  temp = rotateRight(node->right);
  node->right = temp;
  root = rotateLeft(node);
  return root;
}

int calcbalanceFactor(Node *root){
  int leftHeight, rightHeight;
  if(root->left == NULL)
    leftHeight = -1;
  else
    leftHeight = nodeHeight(root->left);
  if(root->right == NULL){
    rightHeight = -1;
  }
  else
    rightHeight = nodeHeight(root->right);
  return (rightHeight-leftHeight);
}

int nodeHeight(Node *root){
  if(root == NULL){
    return -1;
  }
  if(nodeHeight(root->left) > nodeHeight(root->right)){
    return (1+nodeHeight(root->left));
  }
  else{
    return (1+nodeHeight(root->right));
  }
}

/** ------------------------------------------------------------------
 *           before         |        |        after         |
 *                    grand |        |                grand | height
 *     root   child   child | action | root   child   child | change
 *  ------------------------------------------------------------------
 *      +2     +1       x   |   L    |  0       0       x   |  TRUE
 *      +2      0       x   |   L    |  1      -1       x   |  F
 *      +2     -1      -1   |   RL   |  0       1       0   |  TRUE
 *      +2     -1       0   |   RL   |  0       0       0   |  F
 *      +2     -1      +1   |   RL   | -1       0       0   |  F
 *      -2     -1       x   |   R    |  0       0       x   |  F
 *      -2      0       x   |   R    | -1       1       x   |  F
 *      -2     +1      +1   |   LR   |  0      -1       0   |  F
 *      -2     +1       0   |   LR   |  0       0       0   |  F
 *      -2     +1      -1   |   LR   |  1       0       0   |  F
 *  -----------------------------------------------------------------
 */

 /**
  * @brief  balance right side of the tree pointed by rootptr
  * @param  **rootPtr: pointer to the avl tree
  * @retval height change status after avl rotation(if any)
  */
int avlBalanceRightTree(Node **rootPtr)
{

  if ((*rootPtr)->balanceFactor <= 1)
    return CHANGED;

  //need to balance here as balance factor = 2
  if ((*rootPtr)->right->balanceFactor < 0)
  {
    //rotate right left
    (*rootPtr) = rotateRightLeft(*rootPtr);
    switch ((*rootPtr)->balanceFactor)
    {
    case -1:
      (*rootPtr)->balanceFactor = 0;
      (*rootPtr)->left->balanceFactor = 0;
      (*rootPtr)->right->balanceFactor = 1;
      return CHANGED;
      break;
    case 0:
      (*rootPtr)->balanceFactor = 0;
      (*rootPtr)->left->balanceFactor = 0;
      (*rootPtr)->right->balanceFactor = 0;
      return CHANGED;
      break;
    case 1:
      (*rootPtr)->balanceFactor = 0;
      (*rootPtr)->left->balanceFactor = -1;
      (*rootPtr)->right->balanceFactor = 0;
      return CHANGED;
      break;
    }
  }
  else
  {
    //rotate left
    (*rootPtr) = rotateLeft(*rootPtr);
    switch ((*rootPtr)->balanceFactor)
    {
    case 1:
      (*rootPtr)->balanceFactor = 0;
      (*rootPtr)->left->balanceFactor = 0;
      return CHANGED;
      break;
    case 0:
      (*rootPtr)->balanceFactor = -1;
      (*rootPtr)->left->balanceFactor = 1;
      return NO_CHANGED;
      break;
    }
  }
}

/**
  * @brief  balance left side of the tree pointed by rootptr
  * @param  **rootPtr: pointer to the avl tree
  * @retval height change status after avl rotation(if any)
  */
int avlBalanceLeftTree(Node **rootPtr)
{
  if ((*rootPtr)->balanceFactor >= -1)
    return CHANGED;
  if ((*rootPtr)->left->balanceFactor > 0)
  {
    //rotate left right
    (*rootPtr) = rotateleftRight(*rootPtr);
    switch ((*rootPtr)->balanceFactor)
    {
    case 1:
      (*rootPtr)->balanceFactor = 0;
      (*rootPtr)->left->balanceFactor = -1;
      (*rootPtr)->right->balanceFactor = 0;
      return CHANGED;
      break;
    case 0:
      (*rootPtr)->balanceFactor = 0;
      (*rootPtr)->left->balanceFactor = 0;
      (*rootPtr)->right->balanceFactor = 0;
      return CHANGED;
      break;
    case -1:
      (*rootPtr)->balanceFactor = 0;
      (*rootPtr)->left->balanceFactor = 0;
      (*rootPtr)->right->balanceFactor = 1;
      return CHANGED;
      break;
    }
  }
  else
  {
    //rotate right
    (*rootPtr) = rotateRight(*rootPtr);
    switch ((*rootPtr)->balanceFactor)
    {
    case -1:
      (*rootPtr)->balanceFactor = 0;
      (*rootPtr)->right->balanceFactor = 0;
	  return CHANGED;
      break;
    case 0:
      (*rootPtr)->balanceFactor = 1;
      (*rootPtr)->right->balanceFactor = -1;
      return NO_CHANGED;
      break;
    }
  }
}
