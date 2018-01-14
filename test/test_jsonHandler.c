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
#include "NodeVerifier.h"

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
    // TODO: complete createJson TEST
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
 *  TempFolder
 *  |stella.txt|
 * 
 *  propertyJson :
 *      stella.txt{
 *          ...
 *      }
 * 
 * expect: no change
 */
void test_updateJson_given_file_and_jsonData_is_same_expect_nochange(void){
    FileProperty fp = createTempFile(TEST_ENV, "stella.txt", 1000);

    FileProperty stella = {.name = "stella,txt", .size = 1000};
    createJsonFileFromFp(TEST_ENV, &stella, 1);

    updateJson(TEST_ENV);

    TEST_ASSERT_JSON_PROPERTY_PATH(TEST_ENV, &fp, 1);
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

    char buffer[256];
    FileProperty stella = {.name="stella,txt", .size=500};
    createJsonFileFromFp(TEST_ENV, &stella, 1);
    
    updateJson((char *)TEST_ENV);


    json_t *json = json_object();
    sprintf(buffer, "%s/%s", TEST_ENV, JSON_FILE_NAME);
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
    FileProperty fp = {.name="stella.txt", .size=500};

    createJsonFileFromFp(TEST_ENV, &fp, 1);

    //create jang.txt in tempFolder
    FileProperty jangFp = createTempFile(TEST_ENV, "jang.txt", 500);

    updateJson((char *)TEST_ENV);

    TEST_ASSERT_JSON_PROPERTY_PATH(TEST_ENV, &jangFp, 1);
}

/** 
 *    tempFolder
 *   |stella.txt| -- DateModified : 6/1/2018
 *                -- size : 500
 * 
 * given: propertyJson :
 *            stella.txt{
 *                  ...
 *                  DateModified : 5/1/2018
 *                  size: 300
 *            }
 */
void test_updateJson_given_propertyJson_date_not_same_expect_updated(void){

    //create .property.json with parameter specified below
    Date dateModifiedPropertyJson = {.year=2018, .month=1, .day=5, .hour=20, .minute=30};
    FileProperty fp = {.name = "stella.txt", .size = 300, .dateModified = dateModifiedPropertyJson};
    createJsonFileFromFp(TEST_ENV, &fp, 1);

    Date dateModifiedActualFile = {.year = 2018, .month = 1, .day = 6, .hour = 20, .minute = 30};
    FileProperty actualFileProp = createTempFileWithDate(TEST_ENV, "stella.txt", 500, dateModifiedActualFile);

    updateJson(TEST_ENV);

    //test json file in TEST_ENV is the same with the actualFileProp created 
    TEST_ASSERT_JSON_PROPERTY_PATH(TEST_ENV, &actualFileProp, 1);
}

/** 
 *    tempFolder
 *   |stella.txt| -- DateModified : 5/1/2018  20:30:30
 *                -- size : 500
 * 
 * given: propertyJson :
 *            stella.txt{
 *                  ...
 *                  DateModified : 5/1/2018 20:30:29
 *                  size: 300
 *            }
 * 
 * expected: propertyJson get updated to 
 *      propertyJson :
 *            stella.txt{
 *                  ...
 *                  DateModified : 5/1/2018 20:30:30
 *                  size: 500
 *            }
 */
void test_updateJson_given_propertyJson_seconds_of_date_not_same_expect_updated(void){
    //create .property.json with parameter specified below
    Date dateModifiedPropertyJson = {.year=2018, .month=1, .day=5, .hour=20, .minute=30, .seconds = 29};
    FileProperty fp = {.name = "stella.txt", .size = 300, .dateModified = dateModifiedPropertyJson};
    createJsonFileFromFp(TEST_ENV, &fp, 1);

    Date dateModifiedActualFile = {.year = 2018, .month = 1, .day = 5, .hour = 20, .minute = 30, .seconds = 30};
    FileProperty actualFileProp = createTempFileWithDate(TEST_ENV, "stella.txt", 500, dateModifiedActualFile);

    updateJson(TEST_ENV);

    //test json file in TEST_ENV is the same with the actualFileProp created 
    TEST_ASSERT_JSON_PROPERTY_PATH(TEST_ENV, &actualFileProp, 1);
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
    sprintf(dummyFolderPath, "%s/%s", TEST_ENV, "dummy");


    mkdir(dummyFolderPath);
    FileProperty stellaFp = createTempFile(dummyFolderPath, "stella.txt", 250);

    updateCreateAllJsonOnFolder(TEST_ENV);

    //test on tempFolder
    TEST_ASSERT_JSON_PROPERTY_PATH(TEST_ENV, &akaikoenFp, 1);

    //test on dummyFolder
    TEST_ASSERT_JSON_PROPERTY_PATH(dummyFolderPath, &stellaFp, 1);
}

/** 
 *   tempFolder      ----->   dummy   
 *   |stella.txt|    |      |usagi.txt|
 *   |jang.txt |     |      |toneko.txt|
 *   | dummy   | --- |      |.property.json|
 * 
 *   property.json is empty
 */
void test_updateCreateAllJsonOnFolder_given_Files_mainFOlder_has_propertyJson_dummyFolder_no_propertyJson(void){
    FileProperty mainFp[] = {createTempFile(TEST_ENV, "stella.txt", 1000),
                            createTempFile(TEST_ENV, "jang.txt", 500)};

    char dummyFolderPath[256];
    sprintf(dummyFolderPath, "%s/%s", TEST_ENV, "dummy");

    mkdir(dummyFolderPath);
    FileProperty dummyFp[] = {createTempFile(dummyFolderPath, "usagi.txt", 1000),
                              createTempFile(dummyFolderPath, "toneko.txt", 500)};

    //create empty .property.json file
    createTempFile(dummyFolderPath, JSON_FILE_NAME, 0);

    updateCreateAllJsonOnFolder(TEST_ENV);

    //test on tempFolder
    TEST_ASSERT_JSON_PROPERTY_PATH(TEST_ENV, mainFp, 2);

    //test on dummyFolder
    TEST_ASSERT_JSON_PROPERTY_PATH(dummyFolderPath, dummyFp, 2);
}

/** 
 *   tempFolder      ----->   dummy   
 *   |stella.txt|    |      |usagi.txt|
 *   |jang.txt |     |      |toneko.txt|
 *   | dummy   | --- |      |.property.json|
 *                               |
 *            -------------------
 *            |
 *   property.json =
 *        sometingElse{
 *              ...
 *          }
 * 
 *  Expect : property.json{
 *              usagi.txt{
 *                  ...
 *              }
 *              toneko.txt{
 *                  ...
 *              }
 *          }
 */
void test_updateCreateAllJsonOnFolder_given_files_updateJson_on_subFolder(void){
    FileProperty mainFp[] = {createTempFile(TEST_ENV, "stella.txt", 1000),
                             createTempFile(TEST_ENV, "jang.txt", 500)};

    char dummyFolderPath[256];
    sprintf(dummyFolderPath, "%s/%s", TEST_ENV, "dummy");

    mkdir(dummyFolderPath);
    FileProperty dummyFp[] = {createTempFile(dummyFolderPath, "usagi.txt", 1000),
                              createTempFile(dummyFolderPath, "toneko.txt", 500)};

    //create .property.json file
    FileProperty propertyJsonFp = {.name = "sometingElse", .size=500};
    createJsonFileFromFp(dummyFolderPath, &propertyJsonFp, 1);

    updateCreateAllJsonOnFolder(TEST_ENV);

    //test on tempFolder
    TEST_ASSERT_JSON_PROPERTY_PATH(TEST_ENV, mainFp, 2);

    //test on dummyFolder
    TEST_ASSERT_JSON_PROPERTY_PATH(dummyFolderPath, dummyFp, 2);
}
