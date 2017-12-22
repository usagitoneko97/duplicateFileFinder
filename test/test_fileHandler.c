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

const char *TEST_ENV = "tempFolder";
void setUp(void){
    //create working tst folder
    mkdir(TEST_ENV);
}

void tearDown(void){}

FILE* createTempFile(const char *path, const char *name){
    char *completePath = (char*)malloc(sizeof(char) * 256);
    strcpy(completePath, path);
    strcat(completePath, "/");
    strcat(completePath, name);

    FILE *filePtr = fopen(completePath, "w");
    return filePtr;
}

void removeTempFile(const char*pathName){
    remove(pathName);
}

void xtest_getNextFile_given_no_file_expect_NULL(void)
{
    FileObj fileObj;
    TEST_ASSERT_NULL(getNextFile(&fileObj, "r"));
}

void test_getNextFile_given_1File_expect_firstFile(void){
    FileObj fileObj;
    fileObj.dr = opendir(TEST_ENV);
    fileObj.path = (char*)malloc(sizeof(char) * 256);
    strcpy(fileObj.path, TEST_ENV);
    createTempFile(TEST_ENV, "test123.txt");

    FILE *nextFile = getNextFile(&fileObj, "r");

    TEST_ASSERT_NOT_NULL(nextFile);
    
        
}

//TODO : create func to remove all files inside a folder, and place it in teardown
//TODO : a way to check the returned FILE *