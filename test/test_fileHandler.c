#include "unity.h"
#include "fileHandler.h"
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


void setUp(void){}

void tearDown(void){}


void test_getNextFile_given_first_call(void)
{
    FileObj fileobj;
    // getNextFile()

    mkdir("tempFolder");
    fileobj.dr = opendir("tempFolder");
    fileobj.path = "tempFolder";
    
    FILE *file = getNextFile(&fileobj, "w");
    TEST_ASSERT_NOT_NULL(file);
}
