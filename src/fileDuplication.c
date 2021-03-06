#include "fileDuplication.h"
#include "jsonHandler.h"


/** 
 * @brief  find the total duplicate file and put it inside a list
 * @param  *path: the path to search
 * @retval a linked list that contains all the duplication 
 */
DuplicationList findDuplicate(const char *path)
{
    //create propertyJson on all the folder
    updateCreateAllJsonOnFolder((char*)path);
    JsonNode *root = NULL;
    DuplicationList duplicateL;
	duplicateL.numberOfDuplication = 0;
    json2Avl(&root, (char*)path, &duplicateL);

    //delete the whole avl tree
    freeJsonNode(root);
    return duplicateL;
}

void listAllDuplication(DuplicationList dl){
    int i, j;
    Item *temp;
    for(i = 0; i < dl.numberOfDuplication; i++){
        temp = dl.list[i].head;
        printf("--------Duplication #%d-----------\n", i+1);
        printf("total numbers of duplication found: %d\n", dl.list[i].len);
        printf("size : %ld bytes\n", ((FileProperty *)(temp->data))->size);
        printf("----------------------------------\n");
        for(j = 0; j < dl.list[i].len; j++){
            if(temp == NULL){
                break;
            }
            printf("%s/", ((FileProperty*)(temp->data))->path);
            printf("%s \n", ((FileProperty*)(temp->data))->name);
            temp = temp->next;
        }
        printf("----------------------------------\n\n");
    }
}