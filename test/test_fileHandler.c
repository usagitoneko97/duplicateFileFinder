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

    FileContent *fcontent = getNextFile(&fileObj, "r");

    TEST_ASSERT_NULL(getNextFile(&fileObj, "r"));
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

    FileContent *fcontent = getNextFile(&fileObj, "r");
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

    FileContent *nextFile = getNextFile(&fileObj, "r");
    TEST_ASSERT_NOT_NULL(nextFile);
    TEST_ASSERT_EQUAL_STRING("test123.txt", nextFile->name);

    nextFile = getNextFile(&fileObj, "r");
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

    FileContent *nextFile = getNextFile(&fileObj, "r");
    TEST_ASSERT_NOT_NULL(nextFile);
    TEST_ASSERT_EQUAL_STRING("stella.txt", nextFile->name);

    nextFile = getNextFile(&fileObj, "r");
    TEST_ASSERT_NOT_NULL(nextFile);
    TEST_ASSERT_EQUAL_STRING("test123.txt", nextFile->name);

    nextFile = getNextFile(&fileObj, "r");
    TEST_ASSERT_NULL(nextFile);

    closedir(fileObj.dr);
}

//------------------------getNextFolder test cases--------------//