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

    FileProperty fpArray[2] = {createTempFile(TEST_ENV, "akaikoen.txt", 1000),
                                createTempFile(TEST_ENV, "zkaikoen.txt", 500)};
    createJson((char *)TEST_ENV);

    TEST_ASSERT_JSON_PROPERTY_PATH(TEST_ENV, fpArray, 2);
}

//---------------------updateJson test cases-------------------------//

void test_updateJson_on_emptyFolder_expect_propertyJson_empty(void){

    //create an empty .property.json
    createTempFile(TEST_ENV, ".property.json", 0);
    updateJson((char *)TEST_ENV);

    char buffer[256];
    sprintf(buffer, "%s/%s", TEST_ENV, JSON_FILE_NAME);
    json_t *json;
    json = json_object_from_file(buffer);

    // object is empty
    TEST_ASSERT_NULL(json_object_iter(json));
}

/** 
 *     tempFolder      
 *    |stella.txt    |
 *    |.property.json|   
 * 
 *   Given :
 *      .property.json is empty
 * 
 *  expect: 
 *          .property.json
 *              stella.txt{
 *                  ...
 *              }
 */

void test_updateJson_given_propertyJson_empty_folder_with_1File_expect_update_stella(void)
{
    FileProperty fp = createTempFile(TEST_ENV, "stella.txt", 1000);
    //create an empty .property.json
    createTempFile(TEST_ENV, ".property.json", 0);

    updateJson((char *)TEST_ENV);

    TEST_ASSERT_JSON_PROPERTY_PATH(TEST_ENV, &fp, 1);
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
    FileProperty fp[] = {createTempFile(TEST_ENV, "stella.txt", 1000), 
                        createTempFile(TEST_ENV, "jang.txt", 500)};
    //create an empty .property.json
    createTempFile(TEST_ENV, ".property.json", 0);

    updateJson((char *)TEST_ENV);

    TEST_ASSERT_JSON_PROPERTY_PATH(TEST_ENV, fp, 2);
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
    TEST_ASSERT_NULL(json_object_iter(json));

    json_object_clear(json);
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

    FileProperty fp;
    fp.name = "stella.txt";
    fp.size = 500;
    fileParent = json_object();
    createJsonObjectFromFileProp(&fp, fileParent);

    char buffer[256];
    sprintf(buffer, "%s/%s", TEST_ENV, JSON_FILE_NAME);
    json_object_to_file(buffer, fileParent);

    //create jang.txt in tempFolder
    FileProperty jangFp = createTempFile(TEST_ENV, "jang.txt", 500);

    updateJson((char *)TEST_ENV);

    TEST_ASSERT_JSON_PROPERTY_PATH(TEST_ENV, &jangFp, 1);
}

//-----------------updateCreateAllJsonOnFolder-----------------\\ 

/** 
 *   tempFolder        -->   dummy   
 *   |akaikoen.txt|    ^   |stella.txt|
 *   |   dummy    | -----|   
 */
void test_updateCreateAllJsonOnFolder_given_1File_1Folder_both_no_propertyJson(void){
    FileProperty akaikoenFp = createTempFile(TEST_ENV, "akaikoen.txt", 1000);

    char dummyFolderPath[256];
    char mainFolderPropertyPath[256];
    char dummyFolderPropertyPath[256];
    sprintf(dummyFolderPath, "%s/%s", TEST_ENV, "dummy");
    sprintf(mainFolderPropertyPath, "%s/%s", TEST_ENV, JSON_FILE_NAME);
    sprintf(dummyFolderPropertyPath, "%s/%s", dummyFolderPath, JSON_FILE_NAME);

    mkdir(dummyFolderPath);
    FileProperty stellaFp = createTempFile(dummyFolderPath, "stella.txt", 250);

    updateCreateAllJsonOnFolder(TEST_ENV);

    //test on tempFolder
    TEST_ASSERT_JSON_PROPERTY_PATH(TEST_ENV, &akaikoenFp, 1);

    //test on dummyFolder
    TEST_ASSERT_JSON_PROPERTY_PATH(dummyFolderPath, &stellaFp, 1);
}