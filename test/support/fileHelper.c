#include <string.h>
#include <stdio.h>
#include "unity.h"
#include "fileHelper.h"
#include "jsonHandler.h"
#include <stdlib.h>


FileProperty createTempFile(const char *path, const char *name, int size)
{
    char completePath[256];
    int randNum;
    strcpy(completePath, path);
    strcat(completePath, "/");
    strcat(completePath, name);

    FILE *filePtr = fopen(completePath, "w");
    //generate file of this size
    if(size != 0){
        fseek(filePtr, 0, SEEK_SET);
        randNum = rand();
        randNum = randNum % 127;
        fputc((char)(randNum), filePtr);
        fseek(filePtr, size - 1, SEEK_SET);
        fputc('\0', filePtr);
    }

    fclose(filePtr);

    FileProperty fp;
    fp.name = (char*)name;
    fp.size = size;
    fp.dateModified = getFileModifiedDate(completePath);
    return fp;

}

void removeTempFile(const char *pathName)
{
    remove(pathName);
}

char *createMessage(char *message, ...)
{
    va_list args;
    char *buffer;
    int length;

    va_start(args, message);

    length = vsnprintf(buffer, 0, message, args);
    buffer = malloc(length + 1);
    vsnprintf(buffer, length + 1, message, args);

    return buffer;
}

/** 
 * @brief  test all filePropertys exists on jsonObject
 * @param  *json:  json object
 * @param  *fp:    array of file property
 * @param  length: total number of file property  
 * @retval None
 */
void testAssertJson(json_t *json, FileProperty *fp, int length, int lineNo)
{
    int i;
    json_t *obj;
    json_t *size;
    json_t *date;
    char *error;
    for(i = 0; i<length; i++){
        //check the length and the number of object in json
        if(json_object_size(json) != length){
            error = createMessage("expected number of json object is %d, but was %d.", length, json_object_size(json));
            UNITY_TEST_FAIL(lineNo, error);
        }
        //check fp name exist inside json
        obj = json_object_get(json, fp[i].name);
        if(obj == NULL){
            error = createMessage("%s doesn't exist in json object!", fp[i].name);
            UNITY_TEST_FAIL(lineNo, error);
        }
        //test assert the size of the file
        size = json_object_get(obj, "size");
        if(size == NULL){
            error = createMessage("size parameter is not available in %s in json object", fp[i].name);
            UNITY_TEST_FAIL(lineNo, error);
        }
        if(fp[i].size != json_integer_value(size)){
            error = createMessage("expected size of object %s to be %d, but was %d", 
                                    fp[i].name, fp[i].size, json_integer_value(size));
            UNITY_TEST_FAIL(lineNo, error);
        }

        //test assert the date modified of the file
        date = json_object_get(obj, "dateModified");
        if(date == NULL){
            error = createMessage("dateModified parameter is not available in %s in json object", fp[i].name);
            UNITY_TEST_FAIL(lineNo, error);
        }

        Date realDate;
        realDate.year = json_integer_value(json_object_get(date, "year"));
        realDate.month = json_integer_value(json_object_get(date, "month"));
        realDate.day = json_integer_value(json_object_get(date, "day"));
        realDate.hour = json_integer_value(json_object_get(date, "hour"));
        realDate.minute = json_integer_value(json_object_get(date, "minute"));

        if (realDate.year != fp->dateModified.year ||
            realDate.month != fp->dateModified.month ||
            realDate.day != fp->dateModified.day ||
            realDate.hour != fp->dateModified.hour ||
            realDate.minute != fp->dateModified.minute)
        {
            error = createMessage("expected dateModifed of object %s is not matched!", fp[i].name);
            UNITY_TEST_FAIL(lineNo, error);
        }
        json_object_clear(obj);
        json_object_clear(size);
    }
}

void testAssertJsonPath(char *path, FileProperty *fp, int length, int lineNo){
    char buffer[256];
    json_t *jsonObject;
    sprintf(buffer, "%s/%s", path, JSON_FILE_NAME);
    jsonObject = json_object_from_file(buffer);
    testAssertJson(jsonObject, fp, length, lineNo);

    json_object_clear(jsonObject);
}

/** 
 * @brief  create a json file with given fp
 * @note   
 * @param  *path: path to the folder to create json file
 * @param  fp: content of the json
 * @retval None
 */
void createJsonFileFromFp(char *path, FileProperty *fp, int length)
{
    int i;
    json_t *propertyJson = json_object();
    for (i = 0; i<length; i++){
        createJsonObjectFromFileProp(fp++, propertyJson);
    }

    createTempFile(path, JSON_FILE_NAME, 0);
    char buffer[256];
    sprintf(buffer, "%s/%s", path, JSON_FILE_NAME);
    json_object_to_file(buffer, propertyJson);

    json_object_clear(propertyJson);
}