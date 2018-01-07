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
#include "AddAvl.h"
#include "NodeHelper.h"
#include "NodeVerifier.h"
#include "rotate.h"
#include "Node.h"
#include "Exception.h"
#include "CException.h"
#include "AvlInteger.h"
#include "AvlString.h"
#include "DeleteAvl.h"

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
 *   expect avl:
 *                12345
 *              stella.txt
 */
void test_json2Avl_given_propertyJson_stella_expect_avl_stella(void)
{
    FileProperty propertyJsonFp = {.name = "stella.txt", .size=500, .crc=12345};
    createJsonFileFromFp(TEST_ENV, &propertyJsonFp, 1);

    JsonNode *jsonRoot = NULL;
    json2Avl(&jsonRoot, TEST_ENV);

    TEST_ASSERT_NOT_NULL(jsonRoot);
    TEST_ASSERT_EQUAL_STRING("stella.txt", jsonRoot->data->name);
    TEST_ASSERT_EQUAL_NODE(jsonRoot, NULL, NULL, 0);
}

/**
 *    propertyJson :
 *      stella.txt{
 *          crc = 5
 *          ...
 *      }    
 *      jang.txt{
 *          crc = 10
 *          ...
 *      }
 *      akai.txt{
 *          crc = 15
 *          ...
 *      }
 * 
 *   expect avl :
 *         10
 *        /  \
 *       5   15
 */
void test_json2Avl_given_propertyJson_3_object_expect_balanced_avl(void)
{
    FileProperty propertyJsonFp[] = {{.name = "stella.txt", .size = 500, .crc = 5},
                                    {.name = "jang.txt", .size = 500, .crc = 10},
                                    {.name = "akai.txt", .size = 500, .crc = 15}};
    createJsonFileFromFp(TEST_ENV, propertyJsonFp, 3);

    JsonNode *jsonRoot = NULL;
    json2Avl(&jsonRoot, TEST_ENV);

    TEST_ASSERT_NOT_NULL(jsonRoot);
    TEST_ASSERT_EQUAL_STRING("jang.txt", jsonRoot->data->name);
    TEST_ASSERT_EQUAL_STRING("stella.txt", jsonRoot->left->data->name);
    TEST_ASSERT_EQUAL_STRING("akai.txt", jsonRoot->right->data->name);
}
