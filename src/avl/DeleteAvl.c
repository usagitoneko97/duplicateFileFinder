#include "DeleteAvl.h"
#include <stdio.h>
#include "rotate.h"

/** 
 * @brief  perform deletion by given the data
 * @note   wrapper around _avldelete and is intended to use public (public)
 * @param  **rootPtr: pointer to the avl tree
 * @param  data:      data to be deleted
 * @retval non null: the deleted value
 *         null    : data to be deleted doesn't exost in the avl tree
 */
Node *avlDelete(Node **rootPtr, int data){
    int heightChangedTemp;
    Node *deletedNode = _avlDelete(rootPtr, &data, &heightChangedTemp);
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
Node *avlDeleteNearestLeft(Node **rootPtr, int *heightChangedStatus){
    if((*rootPtr) == NULL){
        *heightChangedStatus = CHANGED;
         return NULL;
    }
    if((*rootPtr)->right != NULL){
        tempNode = avlDeleteNearestLeft(&(*rootPtr)->right, heightChangedStatus);
        if(*heightChangedStatus == CHANGED){
            (*rootPtr)->bf--;
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
Node *_avlDelete(Node **rootPtr, void *data, int *heightChangedStatus){
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
                (*rootPtr)->bf --;
                (*rootPtr)->right->bf = (*rootPtr)->bf ++;
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
                (*rootPtr)->bf++;
                (*rootPtr)->left->bf = (*rootPtr)->bf;
                (*rootPtr) = (*rootPtr)->left;
                *heightChangedStatus =  CHANGED;
                return temp;
            }
            //when node that to be deleted has both child right and left
            else{
                //NOTE :heightChangedStatus that pass in here can be any value
                temp = avlDeleteNearestLeft(&(*rootPtr)->left, heightChangedStatus);

				//temp1 = temp;
                //case where nearest node that found is at left of rootptr, then the node 
                //that found should retain its left child instead of overwrite by rootptr right child
                if((*rootPtr)->left != NULL){
                    temp->left = (*rootPtr)->left;
                    temp->right = (*rootPtr)->right;
                }
                else{
                    temp->right = (*rootPtr)->right;
                }

                //modify balance factor by heightChangedStatus
                if(*heightChangedStatus == CHANGED){
                    (*rootPtr)->bf ++;
                }
                //replace bf of node that found to the original bf
                temp->bf = (*rootPtr)->bf;

                //reattach the node that gets deleted
                (*rootPtr) = temp; 

                //balance right tree if needed
                *heightChangedStatus = avlBalanceRightTree(rootPtr);

                //condition where deletion wont cause reduce in height
                if ((*rootPtr)->bf != 0)
                {
                    *heightChangedStatus = NO_CHANGED;
                }
            }
            return temp;
        }
		return NULL;
    }
    else if (compare(data, (*rootPtr)) == 1){
        temp = _avlDelete(&(*(rootPtr))->right, data, heightChangedStatus);
    
        if (*heightChangedStatus == CHANGED)
        {
            (*rootPtr)->bf--;
            *heightChangedStatus = avlBalanceLeftTree(rootPtr);
            
        }
        else{
            heightChangedStatus = NO_CHANGED;
        }
        return temp;
    }
    else{
        temp = _avlDelete(&(*(rootPtr))->left, data, heightChangedStatus);
        if (*heightChangedStatus == CHANGED)
        {
            (*rootPtr)->bf++;
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


Node *avlFindMin(Node *root){
    if(root->left == NULL)
        return root;
    return avlFindMin(root->left);
}

Node *avlFindMax(Node *root)
{
    if (root->right == NULL)
        return root;
    return avlFindMin(root->right);
}