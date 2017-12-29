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

#include <json-c/json.h>

char *TEST_ENV = "tempFolder";
void setUp(void){
    //create working tst folder
    mkdir(TEST_ENV);
}

void tearDown(void){
    //remove test folder after every test
    removeDir(TEST_ENV);
}

void createTempFile(const char *path, const char *name){
    char *completePath = (char*)malloc(sizeof(char) * 256);
    strcpy(completePath, path);
    strcat(completePath, "/");
    strcat(completePath, name);

    FILE *filePtr = fopen(completePath, "w");
    fclose(filePtr);
}

void removeTempFile(const char*pathName){
    remove(pathName);
}




void test_removeDir_given_2_file_expect_empty(void){
    createTempFile(TEST_ENV, "test123.txt");
    createTempFile(TEST_ENV, "test456.txt");

    removeDir(TEST_ENV);

    DIR *dir = opendir(TEST_ENV);
    TEST_ASSERT_NULL(dir);
}

void test_removeDir_given_1File_1Folder_expect_empty(void){
    createTempFile(TEST_ENV, "test123.txt");
    char buffer[256];
    sprintf(buffer, "%s/%s", TEST_ENV, "dummy");
    mkdir(buffer);
    removeDir(TEST_ENV);

    DIR *dir = opendir(TEST_ENV);
    TEST_ASSERT_NULL(dir);
}

void test_removeDir_given_1File_1Folder_with_1File_expect_empty(void){
    createTempFile(TEST_ENV, "test123.txt");
    char buffer[256];
    sprintf(buffer, "%s/%s", TEST_ENV, "dummy");
    mkdir(buffer);
    //create file inside dummy
    createTempFile(buffer, "test123.txt");

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

    FileContent *fcontent = getNextFile(&fileObj);

    TEST_ASSERT_NULL(getNextFile(&fileObj));
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
    createTempFile(TEST_ENV, "test123.txt");

    FileContent *fcontent = getNextFile(&fileObj);
    TEST_ASSERT_EQUAL_STRING("test123.txt", fcontent->name);
    closedir(fileObj.dr);

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

    createTempFile(TEST_ENV, "test123.txt");
    
    char buffer[256];
    sprintf(buffer, "%s/%s", TEST_ENV, "dummy");
    mkdir(buffer);
    createTempFile(buffer, "test456.txt");

    FileContent *nextFile = getNextFile(&fileObj);
    TEST_ASSERT_NOT_NULL(nextFile);
    TEST_ASSERT_EQUAL_STRING("test123.txt", nextFile->name);

    nextFile = getNextFile(&fileObj);
    TEST_ASSERT_NULL(nextFile);

    closedir(fileObj.dr);
}


void test_getNextFile_given_2File_1folder_2FIle_expect_get_only_files_on_that_folder(void)
{
    FileObj fileObj;
    fileObj.dr = opendir(TEST_ENV);
    fileObj.path = (char *)malloc(sizeof(char) * 256);
    strcpy(fileObj.path, TEST_ENV);

    createTempFile(TEST_ENV, "test123.txt");
    createTempFile(TEST_ENV, "stella.txt");
    char buffer[256];
    sprintf(buffer, "%s/%s", TEST_ENV, "dummy");
    mkdir(buffer);
    createTempFile(buffer, "test456.txt");
    createTempFile(buffer, "test789.txt");

    FileContent *nextFile = getNextFile(&fileObj);
    TEST_ASSERT_NOT_NULL(nextFile);
    TEST_ASSERT_EQUAL_STRING("stella.txt", nextFile->name);

    nextFile = getNextFile(&fileObj);
    TEST_ASSERT_NOT_NULL(nextFile);
    TEST_ASSERT_EQUAL_STRING("test123.txt", nextFile->name);

    nextFile = getNextFile(&fileObj);
    TEST_ASSERT_NULL(nextFile);

    closedir(fileObj.dr);
}

//------------------------getNextFolder test cases--------------//
void test_getNextFolder_given_noFolder_expect_return_null(void){
    FileObj fileObj;
    fileObj.dr = opendir(TEST_ENV);
    fileObj.path = (char *)TEST_ENV;
    FolderContent *nextFile = getNextFolder(&fileObj);
    TEST_ASSERT_NULL(nextFile);
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

    nextFolder = getNextFolder(&fileObj);
    TEST_ASSERT_NOT_NULL(nextFolder);
    TEST_ASSERT_EQUAL_STRING("dummy1", nextFolder->name);

    closedir(fileObj.dr);
}

//-----------------------updateJson test cases--------------//

/** 
 *    tempFolder              dummy
 *   |test123.txt|      --> |test123.txt|    
 *   |   dummy   | -----|  
 */
void test_updateJson_given_1File_1Folder_1File(void){
	FileObj fileObj;
    fileObj.dr = opendir(TEST_ENV);
    fileObj.path = (char *)malloc(sizeof(char) * 256);
    strcpy(fileObj.path, TEST_ENV);

    createTempFile(TEST_ENV, "test123.txt");
    
    char buffer[256];
    sprintf(buffer, "%s/%s", TEST_ENV, "dummy");
    mkdir(buffer);
    createTempFile(buffer, "test456.txt");
    updateJson((char*)TEST_ENV);

    closedir(fileObj.dr);
}

void test_updateJson_given_1File_2Folder_2File(void)
{
    FileObj fileObj;
    fileObj.dr = opendir(TEST_ENV);
    fileObj.path = (char *)TEST_ENV;
    createTempFile(TEST_ENV, "akaikoen.txt");
	createTempFile(TEST_ENV, "zkaikoen.txt");

    char buffer[256];
    sprintf(buffer, "%s/%s", TEST_ENV, "dummy");
    mkdir(buffer);
    createTempFile(buffer, "stella.txt");

    sprintf(buffer, "%s/%s", TEST_ENV, "dummy1");
    mkdir(buffer);
    createTempFile(buffer, "jang.txt");

    updateJson((char *)TEST_ENV);

    sprintf(buffer, "%s/%s", TEST_ENV, JSON_FILE_NAME);
    json_object *json = json_object_from_file(buffer);
    json_object *akaikoenJson = json_object_object_get(json, "akaikoen.txt");
    json_object *jsonSize = json_object_object_get(json, "size");
    TEST_ASSERT_NOT_NULL(akaikoenJson);
    printf("json size : %s \n", json_object_to_json_string(jsonSize));

    closedir(fileObj.dr);
}

void test_jsonObjectToFile(void){
    json_object *json;
    json = json_object_new_object(); //create json object
    json_object_object_add(json, "title",
                           json_object_new_string("testies")); //add "title" : "testies"
    json_object_object_add(json, "body", json_object_new_string("testies ... testies ... 1,2,3"));
    json_object_object_add(json, "userId", json_object_new_int(133));
    json_object_to_file("out.json", json);
}