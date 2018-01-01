#include "unity.h"
#include "fileHandler.h"
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <malloc.h>
#include <jansson.h>
#include "_jansson.h"

char *TEST_ENV = "tempFolder";
void setUp(void){
    //create working tst folder
    mkdir(TEST_ENV);
}

void tearDown(void){
    //remove test folder after every test
    removeDir(TEST_ENV);
}

void createTempFile(const char *path, const char *name, int size){
    char completePath[256];
    strcpy(completePath, path);
    strcat(completePath, "/");
    strcat(completePath, name);

    FILE *filePtr = fopen(completePath, "w");
    //generate file of this size
    fseek(filePtr, size-1, SEEK_SET);
    fputc('\0', filePtr);

    fclose(filePtr);
}

void removeTempFile(const char*pathName){
    remove(pathName);
}




void test_removeDir_given_2_file_expect_empty(void){
    createTempFile(TEST_ENV, "test123.txt", 0);
    createTempFile(TEST_ENV, "test456.txt", 0);

    removeDir(TEST_ENV);

    DIR *dir = opendir(TEST_ENV);
    TEST_ASSERT_NULL(dir);
}

void test_removeDir_given_1File_1Folder_expect_empty(void){
    createTempFile(TEST_ENV, "test123.txt", 0);
    char buffer[256];
    sprintf(buffer, "%s/%s", TEST_ENV, "dummy");
    mkdir(buffer);
    removeDir(TEST_ENV);

    DIR *dir = opendir(TEST_ENV);
    TEST_ASSERT_NULL(dir);
}

void test_removeDir_given_1File_1Folder_with_1File_expect_empty(void){
    createTempFile(TEST_ENV, "test123.txt", 0);
    char buffer[256];
    sprintf(buffer, "%s/%s", TEST_ENV, "dummy");
    mkdir(buffer);
    //create file inside dummy
    createTempFile(buffer, "test123.txt", 0);

    removeDir(TEST_ENV);
    DIR *dir = opendir(TEST_ENV);
    TEST_ASSERT_NULL(dir);
}

//---------------------getNextFile test cases-------------------------//

/** 
 *      tempFolder
 *     |          |
 */
void test_getNextFile_given_no_file_expect_NULL(void)
{
    FileObj fileObj;
    fileObj.dr = opendir(TEST_ENV);
    fileObj.path = (char *)malloc(sizeof(char) * 256);
    strcpy(fileObj.path, TEST_ENV);

    FileProperty *fcontent = getNextFile(&fileObj);

    TEST_ASSERT_NULL(fcontent);

    free(fcontent);
    free(fileObj.path);
}
/** 
 *     tempFolder
 *   |test123.txt|
 * 
 */
void test_getNextFile_given_1File_expect_firstFile(void)
{
    FileObj fileObj;
    fileObj.dr = opendir(TEST_ENV);
    fileObj.path = (char *)malloc(sizeof(char) * 256);
    strcpy(fileObj.path, TEST_ENV);
    createTempFile(TEST_ENV, "test123.txt", 0);

    FileProperty *fcontent = getNextFile(&fileObj);
    TEST_ASSERT_EQUAL_STRING("test123.txt", fcontent->name);
    closedir(fileObj.dr);
    free(fcontent);
    free(fileObj.path);

    // remove(TEST_ENV);
}

/** 
 *    tempFolder              dummy
 *   |test123.txt|      --> |test123.txt|    
 *   |   dummy   | -----|  
 */
void test_getNextFile_given_1File_1folder_1FIle_expect_get_all_files(void){
    FileObj fileObj;
    fileObj.dr = opendir(TEST_ENV);
    fileObj.path = (char *)malloc(sizeof(char) * 256);
    strcpy(fileObj.path, TEST_ENV);

    createTempFile(TEST_ENV, "test123.txt", 0);
    
    char buffer[256];
    sprintf(buffer, "%s/%s", TEST_ENV, "dummy");
    mkdir(buffer);
    createTempFile(buffer, "test456.txt", 0);

    FileProperty *nextFile = getNextFile(&fileObj);
    TEST_ASSERT_NOT_NULL(nextFile);
    TEST_ASSERT_EQUAL_STRING("test123.txt", nextFile->name);
    free(nextFile);
    nextFile = getNextFile(&fileObj);
    TEST_ASSERT_NULL(nextFile);
    free(nextFile);
    free(fileObj.path);

    closedir(fileObj.dr);
}


void test_getNextFile_given_2File_1folder_2FIle_expect_get_only_files_on_that_folder(void)
{
    FileObj fileObj;
    fileObj.dr = opendir(TEST_ENV);
    fileObj.path = (char *)malloc(sizeof(char) * 256);
    strcpy(fileObj.path, TEST_ENV);

    createTempFile(TEST_ENV, "test123.txt", 0);
    createTempFile(TEST_ENV, "stella.txt", 0);
    char buffer[256];
    sprintf(buffer, "%s/%s", TEST_ENV, "dummy");
    mkdir(buffer);
    createTempFile(buffer, "test456.txt", 0);
    createTempFile(buffer, "test789.txt", 0);

    FileProperty *nextFile = getNextFile(&fileObj);
    TEST_ASSERT_NOT_NULL(nextFile);
    TEST_ASSERT_EQUAL_STRING("stella.txt", nextFile->name);

    nextFile = getNextFile(&fileObj);
    TEST_ASSERT_NOT_NULL(nextFile);
    TEST_ASSERT_EQUAL_STRING("test123.txt", nextFile->name);
    free(nextFile);

    nextFile = getNextFile(&fileObj);
    TEST_ASSERT_NULL(nextFile);
    free(nextFile);

    closedir(fileObj.dr);
    free(fileObj.path);
}

//------------------------getNextFolder test cases--------------//
void test_getNextFolder_given_noFolder_expect_return_null(void){
    FileObj fileObj;
    fileObj.dr = opendir(TEST_ENV);
    fileObj.path = (char *)TEST_ENV;
    FolderContent *nextFile = getNextFolder(&fileObj);
    TEST_ASSERT_NULL(nextFile);
    free(nextFile);
}

void test_getNextFolder_given_1folder_expect_return_folder(void){
    FileObj fileObj;
    fileObj.dr = opendir(TEST_ENV);
    fileObj.path = (char *)TEST_ENV;

    char buffer[256];
    sprintf(buffer, "%s/%s", TEST_ENV, "dummy");
    mkdir(buffer);

    FolderContent *nextFolder = getNextFolder(&fileObj);
    TEST_ASSERT_NOT_NULL(nextFolder);
    TEST_ASSERT_EQUAL_STRING("dummy", nextFolder->name);

    closedir(fileObj.dr);
    free(nextFolder);
}

void test_get_nextFolder_given_2folder_expect_return_2Folder(void){
    FileObj fileObj;
    fileObj.dr = opendir(TEST_ENV);
    fileObj.path = (char *)TEST_ENV;

    char buffer[256];
    sprintf(buffer, "%s/%s", TEST_ENV, "dummy");
    mkdir(buffer);
    sprintf(buffer, "%s/%s", TEST_ENV, "dummy1");
    mkdir(buffer);

    FolderContent *nextFolder = getNextFolder(&fileObj);
    TEST_ASSERT_NOT_NULL(nextFolder);
    TEST_ASSERT_EQUAL_STRING("dummy", nextFolder->name);
    free(nextFolder);
    nextFolder = getNextFolder(&fileObj);
    TEST_ASSERT_NOT_NULL(nextFolder);
    TEST_ASSERT_EQUAL_STRING("dummy1", nextFolder->name);
    free(nextFolder);

    closedir(fileObj.dr);
}

//-----------------------createJson test cases--------------//

/** 
 *    tempFolder              dummy
 *   |test123.txt|      --> |test123.txt|    
 *   |   dummy   | -----|  
 */
void xtest_createJson_given_1File_1Folder_1File(void){
    createTempFile(TEST_ENV, "test123.txt", 23);
    
    char buffer[256];
    sprintf(buffer, "%s/%s", TEST_ENV, "dummy");
    mkdir(buffer);
    createTempFile(buffer, "test456.txt", 23);
    createJson((char*)TEST_ENV);

}


void test_createJson_given_1File_2Folder_2File(void)
{
    createTempFile(TEST_ENV, "akaikoen.txt", 1000);
	createTempFile(TEST_ENV, "zkaikoen.txt", 500);

    char buffer[256];
    sprintf(buffer, "%s/%s", TEST_ENV, "dummy");
    mkdir(buffer);
    createTempFile(buffer, "stella.txt", 250);

    sprintf(buffer, "%s/%s", TEST_ENV, "dummy1");
    mkdir(buffer);
    createTempFile(buffer, "jang.txt", 500);

    createJson((char *)TEST_ENV);

    sprintf(buffer, "%s/%s", TEST_ENV, JSON_FILE_NAME);
    json_t *json = json_object();
    json = json_object_from_file(buffer);
    json_t *akaikoenJson = json_object_get(json, "akaikoen.txt");
    json_t *jsonSize = json_object_get(akaikoenJson, "size");

    TEST_ASSERT_NOT_NULL(akaikoenJson);
    TEST_ASSERT_NOT_NULL(jsonSize);
    TEST_ASSERT_EQUAL(1000, json_integer_value(jsonSize));

    json_decref(json);
    json_decref(akaikoenJson);
    json_decref(jsonSize);

}

/** 
 *     tempFolder      
 *    |stella.txt    |
 *    |.property.json|   
 * 
 *   Given :
 *      .property.json is empty
 */

 void test_updateJson_given_propertyJson_empty_folder_with_1File_expect_update_stella(void){
     createTempFile(TEST_ENV, "stella.txt", 1000);
     //create an empty .property.json
     createTempFile(TEST_ENV, ".property.json", 0);
     
     updateJson((char *)TEST_ENV);

     char buffer[256];
     sprintf(buffer, "%s/%s", TEST_ENV, JSON_FILE_NAME);
     json_t *json = json_object();
     json = json_object_from_file(buffer);
     TEST_ASSERT_NOT_NULL(json);
     json_t *stellaJson = json_object_get(json, "stella.txt");
     json_t *jsonSize = json_object_get(stellaJson, "size");

     TEST_ASSERT_NOT_NULL(stellaJson);
     TEST_ASSERT_NOT_NULL(jsonSize);
     TEST_ASSERT_EQUAL(1000, json_integer_value(jsonSize));

 }

 /** 
 *     tempFolder      
 *    |stella.txt    |
 *    |jang.txyt     |
 *    |.property.json|   
 * 
 *   Given :
 *      .property.json is empty
 */
 void test_updateJson_given_propertyJson_empty_inside_2file_expect_propertyJson_loaded(void)
 {
//     DIR *dr = opendir(TEST_ENV);
     createTempFile(TEST_ENV, "stella.txt", 1000);
     createTempFile(TEST_ENV, "jang.txt", 500);
     //create an empty .property.json
     createTempFile(TEST_ENV, ".property.json", 0);

     updateJson((char *)TEST_ENV);

     char buffer[256];
     sprintf(buffer, "%s/%s", TEST_ENV, JSON_FILE_NAME);
     json_t *json = json_object();
     json = json_object_from_file(buffer);
     TEST_ASSERT_NOT_NULL(json);
     json_t *stellaJson = json_object_get(json, "stella.txt");
     json_t *jangJson = json_object_get(json, "jang.txt");
     json_t *jsonSize = json_object_get(stellaJson, "size");
     json_t *jangjsonSize = json_object_get(jangJson, "size");

     TEST_ASSERT_NOT_NULL(stellaJson);
     TEST_ASSERT_NOT_NULL(jangJson);
     TEST_ASSERT_NOT_NULL(jsonSize);
     TEST_ASSERT_NOT_NULL(jangjsonSize);
     TEST_ASSERT_EQUAL(1000, json_integer_value(jsonSize));
     TEST_ASSERT_EQUAL(500, json_integer_value(jangjsonSize));

     json_decref(json);
     json_decref(stellaJson);
     json_decref(jangJson);
     json_decref(jsonSize);
     json_decref(jangjsonSize);

 }

 /** 
 *  tempFolder
 *  |       |
 * 
 * given : .property.json
 *       stella.txt{
 *          .....
 *          }
 * 
 * expect : .property.json empty
 */
 void test_updateJson_given_propertyJson_stellaTxt_Folder_empty_expect_propertyJson_empty(void){
    json_t *filePropertiesJson;
    json_t *fileParent;
    fileParent = json_object();
    filePropertiesJson = json_object();
    json_object_set_new(filePropertiesJson, "size",
                        json_integer(500));
    json_object_set_new(fileParent, "stella.txt", filePropertiesJson);

    char buffer[256];
    sprintf(buffer, "%s/%s", TEST_ENV, JSON_FILE_NAME);
    json_object_to_file(buffer, fileParent);

    updateJson((char *)TEST_ENV);
    json_t *json = json_object();
    json = json_object_from_file(buffer);
    json_t *stellaJson = json_object_get(json, "stella.txt");
    TEST_ASSERT_NULL(stellaJson);
 }
/** 
 *  tempFolder         -- Folder1
 *  |stella.txt|      ^   |jang.txt|
 *  |  Folder1 |-----|
 */
void xtest_(){

}



//------------------------dummy test-------------------------//
#include <time.h>

void xtest_jansson_iterate(void){
    json_t *filePropertiesJson;
    json_t *fileParent;
    fileParent = json_object();
    filePropertiesJson = json_object();
    json_object_set_new(filePropertiesJson, "size",
                        json_integer(500));
    json_object_set_new(fileParent, "stella.txt", filePropertiesJson);
    json_object_set_new(filePropertiesJson, "size",
                        json_integer(1000));
    json_object_set_new(fileParent, "jang.txt", filePropertiesJson);
    char buffer[256];
    sprintf(buffer, "%s/%s", TEST_ENV, JSON_FILE_NAME);
    json_object_to_file(buffer, fileParent);

    void *temp;
    const char *key;
    json_t *value;
    json_object_foreach_safe(fileParent, temp, key, value){
        
    }
}

void xtest_jansson_gc(void)
{
    json_t *json = json_object();
    json_t *json1 = json_object();

    json_object_set_new(json, "new", json1);

    json_decref(json);
    TEST_ASSERT_NULL(json1);
}
void delay500ms()
{
    // Converting time into milli_seconds
    int milli_seconds = 100;

    // Stroing start time
    clock_t start_time = clock();

    // looping till required time is not acheived
    while (clock() < start_time + milli_seconds);
}


void xtest_memoryLeak_jansson(void){
    json_t *fileParentJson;
    json_t *filePropertiesJson;

    while(1){
    fileParentJson = json_object();

    filePropertiesJson = json_object();
    json_object_set_new(filePropertiesJson, "size",
                        json_integer(2));
    json_object_set_new(fileParentJson, "file->name", filePropertiesJson);

    filePropertiesJson = json_object();
    json_object_set_new(filePropertiesJson, "size",
                        json_integer(3));
    json_object_set_new(fileParentJson, "name21", filePropertiesJson);

    filePropertiesJson = json_object();
    json_object_set_new(filePropertiesJson, "size",
                        json_integer(3));
    json_object_set_new(fileParentJson, "name21s", filePropertiesJson);

    json_decref(fileParentJson);
    json_decref(filePropertiesJson);

    delay500ms();
    // printf("stella jang!");
    }
}

void dummy1(json_t *json){
    json_error_t *error;
    json = json_load_file("tempFolder/.property.json", JSON_DECODE_ANY, error);
}

void xtest_json_object_from_file_given_no_file(void){
    createTempFile(TEST_ENV, ".property.json", 1);
    json_t json;
    json_error_t *error;
    dummy1(&json);
    // json = json_load_file("tempFolder/.property.json", JSON_DECODE_ANY, error);

    // json = json_object_from_file("tempFolder/.property.json");
    // TEST_ASSERT_NULL(json);
}


/*
void test_jsonObjectToFile(void){
    json_object *json;
    json = json_object_new_object(); //create json object
    json_object_object_add(json, "title",
                           json_object_new_string("testies")); //add "title" : "testies"
    json_object_object_add(json, "body", json_object_new_string("testies ... testies ... 1,2,3"));
    json_object_object_add(json, "userId", json_object_new_int(133));
    json_object_to_file("out.json", json);
}*/