#include "unity.h"
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>
#include <string.h>
#include <malloc.h>
#include <jansson.h>

#include "fileHelper.h"
#include "jsonHandler.h"
#include "fileHandler.h"
#include "_jansson.h"
#include "crc.h"

#include "json2Avl.h"

void setUp(void)
{
    mkdir(TEST_ENV);
}

void tearDown(void)
{
    removeDir(TEST_ENV);
}

/** 
 *    propertyJson : 
 *          stella.txt{
 *              ...
 *          }
 * 
 *   expect : 
 *      avl
 */
void test_json2Avl_given_propertyJson_stella_expect_avl_stella(void)
{
    // FileProperty propertyJsonFp = {.name = "stella.txt", .size=500};
    // createJsonFileFromFp(dummyFolderPath, &propertyJsonFp, 1);
    // json2Avl()
}
