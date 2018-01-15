#include "json2Avl.h"
#include "AddAvl.h"
#include <malloc.h>
#include "Exception.h"
#include "CException.h"
#include "AvlSearch.h"
#include "FileHandler.h"

CEXCEPTION_T ex;

/** 
 * @brief  convert all propertyJson and put it inside avl including sub folder
 * @param  **root: root for avl
 * @param  *path: path of the folder to check
 * @param  *duplicateL: an array of linked list of duplicate files
 * @retval None
 */
void json2Avl(JsonNode **root, char *path, DuplicationList *duplicateL)
{
    //get propertyJson on current folder
    char buffer[256];
    FolderContent *folder;
    json2AvlOnFolder(root, path, duplicateL);
    //get propertyJson on sub folder
    FileObj fileObj;
    loadFileObjWithPath(path, &fileObj);

    folder = getNextFolder(&fileObj);
    while (folder != NULL)
    {
        sprintf(buffer, "%s/%s", path, folder->name);
        json2Avl(root, buffer, duplicateL);
        free(folder);
        folder = getNextFolder(&fileObj);
    }
    fileObj.dr = opendir(path);
    closedir(fileObj.dr);
}

/** 
 * @brief  convert propertyJson and put it inside avl 
 * @note   only convert propertyJson in the current folder
 *          (didnt include the sub folder)
 * @param  **root: root for avl
 * @param  *path: path of the folder to check
 * @param  *duplicateL: an array of linked list of duplicate files
 * @retval None
 */
void json2AvlOnFolder(JsonNode **root, char *path, DuplicationList *duplicateL)
{
    //read json data from .propertyJson just on this path (no recursive)
    char propertJsonPath[256];
    sprintf(propertJsonPath, "%s/%s", path, JSON_FILE_NAME);
    json_t *propertyJson = json_object_from_file(propertJsonPath);

    int duplicateIndex;
    const char *key;
    json_t *value;
    Node *nodeSearched;
    json_object_foreach(propertyJson, key, value){
        //create an avl node
        JsonNode *jsonNode = createNodeWithJson(value, (char*)key);
        // FIXME jsonNode should be free somewhere
        //add to avl
        Try{
            avlAddJsonFp(root, jsonNode, avlCompareFp);
        }Catch(ex){
            nodeSearched = avlSearch((Node *)*root, (void *)&jsonNode->data->crc, avlCompareFpWithCrc);
            if(jsonNode->data->size != ((JsonNode*)nodeSearched)->data->size){
                //size is not same, not a duplicate file
                continue;
            }
            if(duplicateL->numberOfDuplication == 0){
                //first insert
                duplicateL->list = (LinkedList*)malloc(sizeof(LinkedList));
				ListInit(duplicateL->list);

            }
            else{
                duplicateL->list = (LinkedList *)realloc(duplicateL->list,  \
                                                    sizeof(LinkedList) * (duplicateL->numberOfDuplication + 1));
            }
            
            Item *item = createItemWithNode(jsonNode);
            if ((duplicateIndex = searchCrcOnList(duplicateL->list, duplicateL->numberOfDuplication, jsonNode->data->crc)) == -1)
            {
                // this duplicated data is not stored previously in the list
                // search avl on the duplicate node and add into linked list first
                nodeSearched = avlSearch((Node *)*root, (void *)&jsonNode->data->crc, avlCompareFpWithCrc);
                if(nodeSearched == NULL){
                    // something is wrong
                }
                else{
					ListInit(duplicateL->list + duplicateL->numberOfDuplication);
                    Item *itemOnAvl = createItemWithNode((JsonNode*)nodeSearched);
                    ListAddLinkedList(&duplicateL->list[duplicateL->numberOfDuplication],
                                      itemOnAvl);
                }
                ListAddLinkedList(&duplicateL->list[duplicateL->numberOfDuplication],
                                    item);
				duplicateL->numberOfDuplication++;
            }
            else{
                ListAddLinkedList(&duplicateL->list[duplicateIndex],
                                  item);
            }
        }
    }
}

/** 
 * @brief  search whether the crc is exist in the list
 * @param  *list: the list that contains array of duplication
 * @param  numberOfList: total number of duplication
 * @param  crc: the crc to search
 * @retval position of list that contain the crc, -1 if list didnt contain the crc
 */
int searchCrcOnList(LinkedList *list, int numberOfList, int crc){
    int i;
    for(i = 0; i<numberOfList; i++){
        if(((FileProperty*)(list[i].head->data))->crc == crc)
        {
            return i;
        }
    }
    return -1;
}

Item *createItemWithNode(JsonNode *node){
    Item *item = (Item*)malloc(sizeof(Item));
    (item->data) = (void*)node->data;
    item->next = NULL;
    return item;
}

/** 
 * @brief  create a node base on json object
 * @note   json object passed in base must be the file name
 * @param  *json: json object for specific file
 * @retval a json Node that contain all param from json object
 */
JsonNode *createNodeWithJson(json_t *json, char *name){
    JsonNode *jsonNode = (JsonNode *)malloc(sizeof(JsonNode));
    jsonNode->data = (FileProperty *)malloc(sizeof(FileProperty));
    json_t *jsonSize;
    json_t *jsoncrc;
    json_t *jsonDate;
    jsonSize = json_object_get(json, "size");
    jsoncrc = json_object_get(json, "crc");
    jsonDate = json_object_get(json, "date");
    jsonNode->data->size = json_integer_value(jsonSize);
    jsonNode->data->crc = json_integer_value(jsoncrc);
    jsonNode->data->name = name;
    //TODO : add the date
    return jsonNode;
}

int avlCompareFp(void *data1, Node *data2){
    if(((FileProperty*)data1)->crc > ((FileProperty*)(data2->data))->crc){
        return 1;
    }
    else if(((FileProperty*)data1)->crc < ((FileProperty*)(data2->data))->crc){
        return -1;
    }
    else{
        return 0;
    }
}

int avlCompareFpWithCrc(void *data1, Node *data2)
{
    if (*((uint32_t*)data1) > ((FileProperty *)(data2->data))->crc)
    {
        return 1;
    }
    else if (*((uint32_t *)data1) < ((FileProperty *)(data2->data))->crc)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

void freeJsonNode(JsonNode *jsonRoot)
{
    if (jsonRoot == NULL)
    {
        return;
    }
    freeJsonNode(jsonRoot->right);
    freeJsonNode(jsonRoot->left);
    free(jsonRoot->data);
    free(jsonRoot);
}

void freeDuplicationLinkedList(DuplicationList *dupList)
{
    //free item
    //free linked list
    int i;
    for(i = 0; i < dupList->numberOfDuplication; i++){
        Item *iter = (dupList->list + i)->head;
		freeItem(iter);
    }
	free(dupList->list);
   
}

void freeItem(Item *item){
    if(item == NULL){
        return;
    }
    freeItem(item->next);
    free(item);
}
