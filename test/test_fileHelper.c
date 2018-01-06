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

void setUp(void){
    mkdir(TEST_ENV);
}

void tearDown(void){
    removeDir(TEST_ENV);
}

void test_setFileModifiedDate(void){
    char buffer[256];
    createTempFile(TEST_ENV, "modifiedDate.txt", 200);
    sprintf(buffer, "%s/%s", TEST_ENV, "modifiedDate.txt");
    Date date = {2018, 1, 4, 18, 5};
    setFileModifiedDate(buffer, date);
    Date testDate = getFileModifiedDate(buffer);
    TEST_ASSERT_EQUAL_DATE(testDate, date);
}