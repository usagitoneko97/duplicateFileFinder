#include "fileDuplication.h"
#include "jsonHandler.h"


/** 
 * @brief  find the total duplicate file and put it inside a list
 * @param  *path: the path to search
 * @retval a linked list that contains all the duplication 
 */
DuplicationList findDuplicate(char *path)
{
    //create propertyJson on all the folder
    updateCreateAllJsonOnFolder(path);
    JsonNode *root = NULL;
    DuplicationList duplicateL;
	duplicateL.numberOfDuplication = 0;
    json2Avl(&root, path, &duplicateL);

    return duplicateL;
}