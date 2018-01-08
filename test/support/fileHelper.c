#include <string.h>
#include <stdio.h>
#include "unity.h"
#include "fileHelper.h"
#include "jsonHandler.h"
#include <stdlib.h>
#include <sys/stat.h>
#include <utime.h>
#include <time.h>
#include "NodeVerifier.h"

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


void createDuplicateFile(const char *path, char *fileName[], int noOfFile){
    int randNum;
    //limit to ascii code
    randNum = rand() % 127;
    char completePathFile[256];
    int i;
    for(i = 0; i < noOfFile; i++){
        sprintf(completePathFile, "%s/%s", path, *(fileName+i));
        FILE *filePtr = fopen(completePathFile, "w");
        fseek(filePtr, 0, SEEK_SET);
        fputc((char)(randNum), filePtr);
        fseek(filePtr, DUPLICATE_FILE_SIZE - 1, SEEK_SET);
        fputc('\0', filePtr);
        fclose(filePtr);
    }
}

void createFileGivenString(const char *path, char *fileName, char *stringMessage){
    char completePathFile[256];
    sprintf(completePathFile, "%s/%s", path, fileName);

    FILE *filePtr = fopen(completePathFile, "w");
    fseek(filePtr, 0, SEEK_SET);
    fputs(stringMessage, filePtr);
    fseek(filePtr, DUPLICATE_FILE_SIZE - 1, SEEK_SET);
    fputc('\0', filePtr);
    fclose(filePtr);
}


void setFileModifiedDate(char *path, Date date)
{
    struct stat foo;
    time_t modifiedEpoch;
    struct utimbuf new_times;

    date.year -=1900;
    date.month -=1;
    struct tm modifiedDate = {.tm_year=date.year, .tm_mon=date.month, 
                            .tm_mday=date.day, .tm_hour=date.hour, .tm_min=date.minute};
    modifiedEpoch = mktime(&modifiedDate);

    stat(path, &foo);
    
    new_times.actime = foo.st_atime; /* keep atime unchanged */
    new_times.modtime = modifiedEpoch; /* set mtime to current time */
    utime(path, &new_times);
}

FileProperty createTempFileWithDate(const char *path, const char *name, int size, Date date){
    char buffer[256];
    FileProperty fp = createTempFile(path, name, size);
    sprintf(buffer, "%s/%s", path, name);
    setFileModifiedDate((char*)buffer, date);
    fp.dateModified = date;
    return fp;
}

void removeTempFile(const char *pathName)
{
    remove(pathName);
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

void testAssertEqualDate(Date date1, Date date2, int lineNo){
    char *error;
    if (!compare2Date(date1, date2))
    {
        error = createMessage("expected date is not equal to actual date!");
        UNITY_TEST_FAIL(lineNo, error);
    }
}

int compare2Date(Date date1, Date date2){
    if (date1.year != date2.year ||
        date1.month != date2.month ||
        date1.day != date2.day ||
        date1.hour != date2.hour ||
        date1.minute != date2.minute)
    {
        return 0;
    }
    return 1;
}

void testAssertListWithFp(LinkedList *list, FileProperty *fp, int length, int lineNo){
    FileProperty *fp1;
    char *error;
    int i;
    Item *iter = list->head;
    for(i = 0; i<length; i++){
        fp1 = (FileProperty*)(iter->data);
        if(iter == NULL){
            error = createMessage("list does not contain %d amount of data", length);
            UNITY_TEST_FAIL(lineNo, error);
        }
        if (strcmp(fp1->name, (fp+i)->name))
        {
            error = createMessage("expected name to be %s, but was %s", (fp+i)->name, fp1->name);
            UNITY_TEST_FAIL(lineNo, error);
        }
        if (fp1->size != (fp+i)->size)
        {
            error = createMessage("expected size to be %d, but was %d", (fp+i)->size, fp1->size);
            UNITY_TEST_FAIL(lineNo, error);
        }
        if (fp1->crc != (fp+i)->crc)
        {
            error = createMessage("expected name to be %d, but was %d", (fp+i)->crc, fp1->crc);
            UNITY_TEST_FAIL(lineNo, error);
        }
        // if (!compare2Date(fp1->dateModified, fp->dateModified))
        // {
        //     error = createMessage("expected date of %s is not the same with date of %s", fp->name, fp1->name);
        //     UNITY_TEST_FAIL(lineNo, error);
        // }
        iter = iter->next;
    }
}

void testAssertListWithFpOnlyName(LinkedList *list, char*name[], int numOfFile, int lineNo){
    FileProperty *fp1;
    char *error;
    int i;
    Item *iter = list->head;
    for (i = 0; i < numOfFile; i++)
    {
        fp1 = (FileProperty *)(iter->data);
        if (iter == NULL)
        {
            error = createMessage("list does not contain %d amount of data", numOfFile);
            UNITY_TEST_FAIL(lineNo, error);
        }
        if (strcmp(fp1->name, name[i]))
        {
            error = createMessage("expected name to be %s, but was %s", name[i], fp1->name);
            UNITY_TEST_FAIL(lineNo, error);
        }
        iter = iter->next;
    }
}