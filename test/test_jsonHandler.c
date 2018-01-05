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
#include "jsonHandler.h"
#include "fileHandler.h"
#include "_jansson.h"

void setUp(void)
{
    mkdir(TEST_ENV);
}

void tearDown(void)
{
    //remove test folder after every test
    removeDir(TEST_ENV);
}

//-----------------------createJson test cases--------------//

/** 
 *    tempFolder              dummy
 *   |test123.txt|      --> |test123.txt|    
 *   |   dummy   | -----|  
 */
void xtest_createJson_given_1File_1Folder_1File(void)
{
    createTempFile(TEST_ENV, "test123.txt", 23);

    char buffer[256];
    sprintf(buffer, "%s/%s", TEST_ENV, "dummy");
    mkdir(buffer);
    createTempFile(buffer, "test456.txt", 23);
    createJson((char *)TEST_ENV);
}

/** 
 *    tempFolder
 *    |akaikoen.txt|
 *    |zkaikoen.txt|
 * 
 * expect:
 *      .property.json
 *          akaikoen.txt{
 *              ...
 *          }
 *         zkaikoen.txt{
 *              ...
 *          }
 * 
 */
void test_createJson_given_2File_expect_create_json_and_update_file(void)
{
    char buffer[256];
    createTempFile(TEST_ENV, "akaikoen.txt", 1000);
    createTempFile(TEST_ENV, "zkaikoen.txt", 500);

    createJson((char *)TEST_ENV);

    sprintf(buffer, "%s/%s", TEST_ENV, JSON_FILE_NAME);
    json_t *json = json_object();
    json = json_object_from_file(buffer);

    json_t *akaikoenJson = json_object_get(json, "akaikoen.txt");
    TEST_ASSERT_NOT_NULL(akaikoenJson);
    json_t *akaiKoenSize = json_object_get(akaikoenJson, "size");
    TEST_ASSERT_NOT_NULL(akaiKoenSize);

    json_t *zkaikoenJson = json_object_get(json, "zkaikoen.txt");
    TEST_ASSERT_NOT_NULL(zkaikoenJson);
    json_t *zkaiKoenSize = json_object_get(zkaikoenJson, "size");
    TEST_ASSERT_NOT_NULL(zkaiKoenSize);

    TEST_ASSERT_EQUAL(1000, json_integer_value(akaiKoenSize));
    TEST_ASSERT_EQUAL(500, json_integer_value(zkaiKoenSize));

    json_decref(json);
    // json_decref(akaikoenJson);
    // json_decref(jsonSize);
}

//---------------------updateJson test cases-------------------------//

/** 
 *     tempFolder      
 *    |stella.txt    |
 *    |.property.json|   
 * 
 *   Given :
 *      .property.json is empty
 */

void test_updateJson_given_propertyJson_empty_folder_with_1File_expect_update_stella(void)
{
    createTempFile(TEST_ENV, "stella.txt", 1000);
    //create an empty .property.json
    createTempFile(TEST_ENV, ".property.json", 0);

    updateJson((char *)TEST_ENV);

    char buffer[256];
    sprintf(buffer, "%s/%s", TEST_ENV, JSON_FILE_NAME);
    json_t *json;
    json = json_object_from_file(buffer);
    TEST_ASSERT_NOT_NULL(json);
    json_t *stellaJson = json_object_get(json, "stella.txt");
    json_t *jsonSize = json_object_get(stellaJson, "size");

    TEST_ASSERT_NOT_NULL(stellaJson);
    TEST_ASSERT_NOT_NULL(jsonSize);
    TEST_ASSERT_EQUAL(1000, json_integer_value(jsonSize));
}

/** 
 *     tempFolder      
 *    |stella.txt    |
 *    |jang.txyt     |
 *    |.property.json|   
 * 
 *   Given :
 *      .property.json is empty
 */
void test_updateJson_given_propertyJson_empty_inside_2file_expect_propertyJson_loaded(void)
{
    //     DIR *dr = opendir(TEST_ENV);
    createTempFile(TEST_ENV, "stella.txt", 1000);
    createTempFile(TEST_ENV, "jang.txt", 500);
    //create an empty .property.json
    createTempFile(TEST_ENV, ".property.json", 0);

    updateJson((char *)TEST_ENV);

    char buffer[256];
    sprintf(buffer, "%s/%s", TEST_ENV, JSON_FILE_NAME);
    json_t *json = json_object();
    json = json_object_from_file(buffer);
    TEST_ASSERT_NOT_NULL(json);
    json_t *stellaJson = json_object_get(json, "stella.txt");
    json_t *jangJson = json_object_get(json, "jang.txt");
    json_t *jsonSize = json_object_get(stellaJson, "size");
    json_t *jangjsonSize = json_object_get(jangJson, "size");

    TEST_ASSERT_NOT_NULL(stellaJson);
    TEST_ASSERT_NOT_NULL(jangJson);
    TEST_ASSERT_NOT_NULL(jsonSize);
    TEST_ASSERT_NOT_NULL(jangjsonSize);
    TEST_ASSERT_EQUAL(1000, json_integer_value(jsonSize));
    TEST_ASSERT_EQUAL(500, json_integer_value(jangjsonSize));

    // FIXME find out the sub object need to be decref or not
    //   json_decref(jangjsonSize);
    //   json_decref(jsonSize);
    //   json_decref(jangJson);
    //   json_decref(stellaJson);
    json_decref(json);
}

/** 
 *  tempFolder
 *  |       |
 * 
 * given : .property.json
 *       stella.txt{
 *          .....
 *          }
 * 
 * expect : delete stella.txt in .property.json
 */
void test_updateJson_given_propertyJson_stellaTxt_Folder_empty_expect_propertyJson_empty(void)
{
    json_t *filePropertiesJson;
    json_t *fileParent;
    fileParent = json_object();
    filePropertiesJson = json_object();
    json_object_set_new(filePropertiesJson, "size",
                        json_integer(500));
    json_object_set_new(fileParent, "stella.txt", filePropertiesJson);

    char buffer[256];
    sprintf(buffer, "%s/%s", TEST_ENV, JSON_FILE_NAME);
    json_object_to_file(buffer, fileParent);

    updateJson((char *)TEST_ENV);
    json_t *json = json_object();
    json = json_object_from_file(buffer);
    json_t *stellaJson = json_object_get(json, "stella.txt");
    TEST_ASSERT_NULL(stellaJson);
}

/** 
 *    tempFolder
 *    |jang.txt|
 * 
 * given :.property.json
 *      stella.txt{
 *              ...
 *          }
 * expect : .property.json
 *           jang.txt{
 *                  ...
 *              }
 *              
 */
void test_updateJson_given_propertyJson_stellaTxt_Folder_jangTxt_expect_propertyJson_remove_stellaTxt_add_jangTxt(void)
{
    json_t *filePropertiesJson;
    json_t *fileParent;
    fileParent = json_object();
    filePropertiesJson = json_object();
    json_object_set_new(filePropertiesJson, "size",
                        json_integer(500));
    json_object_set_new(fileParent, "stella.txt", filePropertiesJson);

    char buffer[256];
    sprintf(buffer, "%s/%s", TEST_ENV, JSON_FILE_NAME);
    json_object_to_file(buffer, fileParent);

    //create jang.txt in tempFolder
    createTempFile(TEST_ENV, "jang.txt", 500);

    updateJson((char *)TEST_ENV);

    json_t *json = json_object();
    json = json_object_from_file(buffer);
    json_t *stellaJson = json_object_get(json, "stella.txt");
    TEST_ASSERT_NULL(stellaJson);

    json_t *jangJson = json_object_get(json, "jang.txt");
    TEST_ASSERT_NOT_NULL(jangJson);

    json_t *jangjsonSize = json_object_get(jangJson, "size");
    TEST_ASSERT_EQUAL(500, json_integer_value(jangjsonSize));

    json_decref(json);
}
