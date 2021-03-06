#include "DeleteAvl.h"
#include <stdio.h>
#include "rotate.h"

/** 
 * @brief  perform deletion by given the data
 * @note   wrapper around _avlRemove and is intended to use public (public)
 * @param  **rootPtr: pointer to the avl tree
 * @param  data:      data to be deleted
 * @retval non null: the deleted value
 *         null    : data to be deleted doesn't exost in the avl tree
 */
Node *avlRemove(Node **rootPtr, void* data, CompareWithVoidPtr compare)
{
    int heightChangedTemp;
    Node *deletedNode = _avlRemove(rootPtr, data, &heightChangedTemp, compare);
    if(deletedNode == NULL){
        printf("deleted value doesn't exist!");
    }
    return deletedNode;
}   

Node *tempNode;

/** 
 * @brief  delete nearest value (left side) of root
 * @note   heightChangedStatus is only internally use (to balance the tree)
 * @param  **rootPtr:               root ptr of avl struct
 * @param  *heightChangedStatus:    height change (true or false) of the avl
 * @retval deleted node
 */
Node *avlRemoveNearestLeft(Node **rootPtr, int *heightChangedStatus){
    if((*rootPtr) == NULL){
        *heightChangedStatus = CHANGED;
         return NULL;
    }
    if((*rootPtr)->right != NULL){
        tempNode = avlRemoveNearestLeft(&(*rootPtr)->right, heightChangedStatus);
        if(*heightChangedStatus == CHANGED){
            (*rootPtr)->balanceFactor--;
            *heightChangedStatus = avlBalanceLeftTree(rootPtr);
        }
        else{
            return NO_CHANGED;
        }
        if (tempNode != NULL && tempNode->left != NULL)
        {
            //pass grandchild to child before replacing at the parent
            (*rootPtr)->right = tempNode->left;
            tempNode->left = NULL;
        }
        return tempNode;
    }   
    //recursive untill right is null, delete itself
    else{
        tempNode = (*rootPtr);
        (*rootPtr) = NULL;
        *heightChangedStatus = CHANGED;
        return tempNode;
    }
}

/** 
 * @brief  perform deletion 
 * @note   private function 
 * @param  **rootPtr: root ptr of the avl tree
 * @param  *data:                    pointer to the data to be deleted
 * @param  *heightChangedStatus:     status changed of the height of the avl (only internally use)
 * @retval non null: the deleted value
 *         null    : data to be deleted doesn't exost in the avl tree
 */
Node *_avlRemove(Node **rootPtr, void *data, int *heightChangedStatus, CompareWithVoidPtr compare)
{
    int min;
    Node *temp, *temp1;
    if((*rootPtr) == NULL)
        return NULL;
    if(compare(data, (*rootPtr)) == 0){
        if ((*rootPtr)->left == NULL && (*rootPtr)->right == NULL){
            *heightChangedStatus = CHANGED;
            temp = (*rootPtr);
            (*rootPtr) = NULL;
            return temp;
        }
        else{
            //has child
            //if left == NULL, directly move rootptr right up to replace
            if((*rootPtr)->left == NULL){
                //returning node to be deleted
                temp = (*rootPtr);
                (*rootPtr)->balanceFactor --;
                (*rootPtr)->right->balanceFactor = (*rootPtr)->balanceFactor ++;
                (*rootPtr) = (*rootPtr)->right;
                *heightChangedStatus =  CHANGED;
                return temp;
                // temp = avlFindMin((*rootPtr)->right);
                // int heightChanged = 
            }
            //if right == NULL, directly move rootptr left up to replace
            else if((*rootPtr)->right == NULL){
                //removing (*rootPtr);
                //returning node to be deleted
                temp = (*rootPtr);
                (*rootPtr)->balanceFactor++;
                (*rootPtr)->left->balanceFactor = (*rootPtr)->balanceFactor;
                (*rootPtr) = (*rootPtr)->left;
                *heightChangedStatus =  CHANGED;
                return temp;
            }
            //when node that to be deleted has both child right and left
            else{
                temp = avlRemoveNearestLeft(&(*rootPtr)->left, heightChangedStatus);

                //temp1 = temp;
                //case where nearest node that found is at left of rootptr, then the node
                //that found should retain its left child instead of overwrite by rootptr right child
               
                if((*rootPtr)->left != NULL){
                    temp->left = (*rootPtr)->left;
                    temp->right = (*rootPtr)->right;
                }
                else{
                    //see @test_avlRemove_3_expect_rotateAtParent
                    temp->right = (*rootPtr)->right;
                }

                //modify balance factor by heightChangedStatus
                if(*heightChangedStatus == CHANGED){
                    (*rootPtr)->balanceFactor ++;
                }
                //replace balanceFactor of node that found to the original balanceFactor
                temp->balanceFactor = (*rootPtr)->balanceFactor;

                //reattach the node that gets deleted
                (*rootPtr) = temp; 

                //balance right tree if needed
                *heightChangedStatus = avlBalanceRightTree(rootPtr);

                //condition where deletion wont cause reduce in height
                if ((*rootPtr)->balanceFactor != 0)
                {
                    *heightChangedStatus = NO_CHANGED;
                }
            }
            return temp;
        }
		return NULL;
    }
    else if (compare(data, (*rootPtr)) == 1){
        temp = _avlRemove(&(*(rootPtr))->right, data, heightChangedStatus, compare);
    
        if (*heightChangedStatus == CHANGED)
        {
            (*rootPtr)->balanceFactor--;
            *heightChangedStatus = avlBalanceLeftTree(rootPtr);
            
        }
        else{
            heightChangedStatus = NO_CHANGED;
        }
        return temp;
    }
    else{
        temp = _avlRemove(&(*(rootPtr))->left, data, heightChangedStatus, compare);
        if (*heightChangedStatus == CHANGED)
        {
            (*rootPtr)->balanceFactor++;
            *heightChangedStatus = avlBalanceRightTree(rootPtr);
        }
        else
        {
            *heightChangedStatus =  NO_CHANGED;
        }
        return temp;
    }
	return NULL;
}
