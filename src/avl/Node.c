#include "Node.h"

/** 
 * @brief  return 1 when node > data
 * @note   
 * @param  *node: 
 * @param  *data: 
 * @retval result
 */
int compare(void *data, Node *node)
{
    if(*(int*)data > node->data){
        return 1;
    }
    else if (*(int *)data < node->data)
    {
        return -1;
    }
    else{
        return 0;
    }
}