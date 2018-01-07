#include "json2Avl.h"
#include "AddAvl.h"
#include <malloc.h>
#include "Exception.h"
#include "CException.h"

CEXCEPTION_T ex;
#define avlAddJsonFp(jsonRootNode, nodeToAdd, avlCompareFp) \
            _avlAdd((Node **)jsonRootNode, (Node *)nodeToAdd, avlCompareFp)

void json2AvlOnFolder(JsonNode **root, char *path)
{
    //read json data from .propertyJson just on this path (no recursive)
    char propertJsonPath[256];
    sprintf(propertJsonPath, "%s/%s", path, JSON_FILE_NAME);
    json_t *propertyJson = json_object_from_file(propertJsonPath);

    const char *key;
    json_t *value;
    json_object_foreach(propertyJson, key, value){
        //create an avl node
        JsonNode *jsonNode = createNodeWithJson(value, (char*)key);
        // FIXME jsonNode should be free somewhere
        //add to avl
        Try{
            avlAddJsonFp(root, jsonNode, avlCompareFp);
        }Catch(ex){
            //file already exists
            Throw(ex);
        }
    }
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