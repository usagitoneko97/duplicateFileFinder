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
#include "AvlSearch.h"

#include "linkedlist.h"
CEXCEPTION_T ex;
void setUp(void)
{
    mkdir(TEST_ENV);
}

void tearDown(void)
{
    removeDir(TEST_ENV);
}

void test_json2AvlOnFolder_given_propertyJson_empty_expect_emptyAvl(void){
    //create an empty property json file

    createTempFile(TEST_ENV, JSON_FILE_NAME, 0);
    JsonNode *jsonRoot = NULL;
    DuplicationList duplicateL;
    json2AvlOnFolder(&jsonRoot, TEST_ENV, &duplicateL);

    TEST_ASSERT_NULL(jsonRoot);
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
void test_json2AvlOnFolder_given_propertyJson_stella_expect_avl_stella(void)
{
    FileProperty propertyJsonFp = {.name = "stella.txt", .size=500, .crc=12345};
    createJsonFileFromFp(TEST_ENV, &propertyJsonFp, 1);

    DuplicationList duplicateL;
    JsonNode *jsonRoot = NULL;
    json2AvlOnFolder(&jsonRoot, TEST_ENV, &duplicateL);

    TEST_ASSERT_NOT_NULL(jsonRoot);
    TEST_ASSERT_EQUAL_STRING("stella.txt", jsonRoot->data->name);
    TEST_ASSERT_EQUAL_NODE(jsonRoot, NULL, NULL, 0);
    free(jsonRoot);
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
void test_json2AvlOnFolder_given_propertyJson_3_object_expect_balanced_avl(void)
{
    FileProperty propertyJsonFp[] = {{.name = "stella.txt", .size = 500, .crc = 5},
                                    {.name = "jang.txt", .size = 500, .crc = 10},
                                    {.name = "akai.txt", .size = 500, .crc = 15}};
    createJsonFileFromFp(TEST_ENV, propertyJsonFp, 3);
    DuplicationList duplicateL;
    JsonNode *jsonRoot = NULL;
    json2AvlOnFolder(&jsonRoot, TEST_ENV, &duplicateL);

    TEST_ASSERT_NOT_NULL(jsonRoot);
    TEST_ASSERT_EQUAL_STRING("jang.txt", jsonRoot->data->name);
    TEST_ASSERT_EQUAL_STRING("stella.txt", jsonRoot->left->data->name);
    TEST_ASSERT_EQUAL_STRING("akai.txt", jsonRoot->right->data->name);
    free(jsonRoot->right->data);
    free(jsonRoot->right);
    free(jsonRoot->left->data);
    free(jsonRoot->left);
    free(jsonRoot);
}

/**
 *     propertyJson:
 *          quick.txt{
 *              crc=5
 *              ...
 *          }
 *          brown.txt{
 *              crc=10
 *          }
 *          fox.txt{
 *              crc=5
 *          }
 * 
 *   expect avl:
 *          5
 *           \
 *           10
 * 
 * 
 *  expect linkedlist[0] : quick.txt - fox.txt - NULL
 *                           |            |
 *                          head          tail
 */
void test_json2AvlOnFolder_given_propertyJson_3_obj_2_same_crc_expect_update_LL(void){
    FileProperty propertyJsonFp[] = {{.name = "quick.txt", .size = 500, .crc = 5},
                                     {.name = "brown.txt", .size = 500, .crc = 10},
                                     {.name = "fox.txt", .size = 500, .crc = 5}};
    createJsonFileFromFp(TEST_ENV, propertyJsonFp, 3);

    JsonNode *jsonRoot = NULL;
    DuplicationList duplicateL;
    duplicateL.numberOfDuplication = 0;

	json2AvlOnFolder(&jsonRoot, TEST_ENV, &duplicateL);


    TEST_ASSERT_NOT_NULL(duplicateL.list);
    TEST_ASSERT_EQUAL(1, duplicateL.numberOfDuplication);
    TEST_ASSERT_EQUAL_STRING("quick.txt", ((FileProperty *)(duplicateL.list->head->data))->name);

    TEST_ASSERT_NOT_NULL(jsonRoot);
    TEST_ASSERT_NULL(jsonRoot->left);
    TEST_ASSERT_NOT_NULL(jsonRoot->right);
    TEST_ASSERT_EQUAL_STRING("quick.txt", jsonRoot->data->name);
    TEST_ASSERT_EQUAL_STRING("brown.txt", jsonRoot->right->data->name);
    

    free(jsonRoot->right->data);
    free(jsonRoot->right);
    free(jsonRoot);
    // TEST_ASSERT_EQUAL_STRING("brown.txt", jsonRoot->right->data->name);
}

/**
 *     propertyJson:
 *          quick.txt{
 *              crc=5
 *              ...
 *          }
 *          brown.txt{
 *              crc=10
 *          }
 *          fox.txt{
 *              crc=5
 *          }
 *          jumps.txt{
 *              crc=5
 *          }
 *   expect avl:
 *          5
 *           \
 *           10
 * 
 *  exception thrown at fox.txt
 * 
 *  expect linkedlist[0] :quick.txt - fox.txt - jumps.txt - NULL
 *                            |                      |
 *                          head                    tail
 */
void test_json2AvlOnFolder_given_propertyJson_4_obj_3_same_crc_expect_update_LL(void)
{
    FileProperty propertyJsonFp[] = {{.name = "quick.txt", .size = 500, .crc = 5},
                                     {.name = "brown.txt", .size = 500, .crc = 10},
                                     {.name = "fox.txt", .size = 500, .crc = 5},
                                     {.name = "jumps.txt", .size = 500, .crc = 5}};
    createJsonFileFromFp(TEST_ENV, propertyJsonFp, 4);

    JsonNode *jsonRoot = NULL;
    DuplicationList duplicateL;
    duplicateL.numberOfDuplication = 0;

	json2AvlOnFolder(&jsonRoot, TEST_ENV, &duplicateL);

    TEST_ASSERT_NOT_NULL(duplicateL.list);
    TEST_ASSERT_EQUAL(1, duplicateL.numberOfDuplication);
    TEST_ASSERT_EQUAL_STRING("quick.txt", ((FileProperty *)(duplicateL.list->head->data))->name);
    TEST_ASSERT_NOT_NULL(duplicateL.list->head->next);
    TEST_ASSERT_EQUAL_STRING("fox.txt", ((FileProperty *)(duplicateL.list->head->next->data))->name);
    TEST_ASSERT_NOT_NULL(duplicateL.list->head->next->next);
    TEST_ASSERT_EQUAL_STRING("jumps.txt", ((FileProperty *)(duplicateL.list->head->next->next->data))->name);

    TEST_ASSERT_NOT_NULL(jsonRoot);
    TEST_ASSERT_NULL(jsonRoot->left);
    TEST_ASSERT_NOT_NULL(jsonRoot->right);
    TEST_ASSERT_EQUAL_STRING("quick.txt", jsonRoot->data->name);
    TEST_ASSERT_EQUAL_STRING("brown.txt", jsonRoot->right->data->name);

    free(jsonRoot->right->data);
    free(jsonRoot->right);
    free(jsonRoot);
    // TEST_ASSERT_EQUAL_STRING("brown.txt", jsonRoot->right->data->name);
}