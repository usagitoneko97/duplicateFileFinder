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

void xtest_getNextFile_given_no_file_expect_NULL(void)
{
    FileObj fileObj;
    TEST_ASSERT_NULL(getNextFile(&fileObj, "r"));
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

void test_getNextFile_given_1File_expect_firstFile(void)
{
    FileObj fileObj;
    fileObj.dr = opendir(TEST_ENV);
    fileObj.path = (char *)malloc(sizeof(char) * 256);
    strcpy(fileObj.path, TEST_ENV);
    createTempFile(TEST_ENV, "test123.txt");

    FILE *nextFile = getNextFile(&fileObj, "r");
    TEST_ASSERT_NOT_NULL(nextFile);
    fclose(nextFile);
    closedir(fileObj.dr);

    // remove(TEST_ENV);
}

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

    FILE *nextFile = getNextFile(&fileObj, "r");
    TEST_ASSERT_NOT_NULL(nextFile);
    fclose(nextFile);

    nextFile = getNextFile(&fileObj, "r");
    TEST_ASSERT_NOT_NULL(nextFile);
    fclose(nextFile);

    closedir(fileObj.dr);
}
//TODO : a way to check the returned FILE *