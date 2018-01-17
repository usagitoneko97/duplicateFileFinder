#include "unity.h"
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <malloc.h>
#include <jansson.h>

#include "fileHelper.h"
#include "fileHandler.h"
#include "jsonHandler.h"
#include "_jansson.h"
#include "crc.h"
#include "NodeVerifier.h"

void setUp(void){
    //create working tst folder
    mkdir(TEST_ENV);
}

void tearDown(void){
    //remove test folder after every test
    removeDir(TEST_ENV);
}

/** 
 *  tempFolder
 *  |test123.txt|
 *  |test456.txt|
 * 
 *  removeDir(tempFolder);
 * 
 * expect: tempFolder does not exist
 */
void test_removeDir_given_2_file_expect_empty(void){
    createTempFile(TEST_ENV, "test123.txt", 0);
    createTempFile(TEST_ENV, "test456.txt", 0);

    removeDir(TEST_ENV);

    DIR *dir = opendir(TEST_ENV);
    TEST_ASSERT_NULL(dir);
}

/** 
 *  tempFolder          ---->   dummy
 *  |test123.txt|      |      |        |
 *  |   dummy   | -----|
 * 
 *  removeDir(tempFolder);
 * 
 * expect: tempFolder and dummy folder does not exist
 */
void test_removeDir_given_1File_1Folder_expect_empty(void){
    createTempFile(TEST_ENV, "test123.txt", 0);
    char buffer[256];
    sprintf(buffer, "%s/%s", TEST_ENV, "dummy");
    mkdir(buffer);
    removeDir(TEST_ENV);

    DIR *dir = opendir(TEST_ENV);
    TEST_ASSERT_NULL(dir);
}

/** 
 *  tempFolder          ---->    dummy
 *  |test123.txt|      |      |test123.txt|
 *  |   dummy   | -----|
 * 
 *  removeDir(tempFolder);
 * 
 * expect: tempFolder and dummy folder does not exist
 */
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

/** 
 *   tempFolder
 *       -test123.txt
 *       -stella.txt
 *       dummy
 *          -test456.txt
 *          -test789.txt
 * 
 * expect: getnextFile will return file in order :
 *              stella.txt
 *              test123.txt
 *              NULL
 */
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

/** 
 *  tempFolder
 *      -
 * 
 * expect : get next folder return NULL as no folder contains inside tempFolder
 */
void test_getNextFolder_given_noFolder_expect_return_null(void){
    FileObj fileObj;
    fileObj.dr = opendir(TEST_ENV);
    fileObj.path = (char *)TEST_ENV;
    FolderContent *nextFile = getNextFolder(&fileObj);
    TEST_ASSERT_NULL(nextFile);
    free(nextFile);
}

/** 
 *   tempFolder
 *      -dummy
 * 
 *  expect: get next folder return dummy
 */
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

/** 
 *  tempFolder
 *      -dummy
 *      -dummy1
 * 
 * expect : get next folder return folder (in order):
 *          dummy1
 *          dummy2
 */
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