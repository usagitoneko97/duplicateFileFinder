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

#include "fileDuplication.h"
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

void setUp(void)
{
    mkdir(TEST_ENV);
}

void tearDown(void)
{
    //remove test folder after every test
    removeDir(TEST_ENV);
}

/** 
 *        tempFolder
 *       | brown.txt| --- duplicate(1)
 *       | quick.txt| --- duplicate(2)
 *       | the.txt  | 
 *       | fox.txt  |
 *       | jumps.txt|
 * 
 *  expect : linkedList : 
 *          brown.txt  -  quick.txt  -- NULL
 *             |             |
 *            Head          Tail
 */
void test_fileDuplication_given_2_files_are_duplicate_expect_return_dupList(void)
{
    char *fileName[] = {"quick.txt", "brown.txt"};
    createDuplicateFile(TEST_ENV, fileName, 2);
    createTempFile(TEST_ENV, "the.txt", 20);
    createTempFile(TEST_ENV, "fox.txt", 20);
    createTempFile(TEST_ENV, "jumps.txt", 20);

    DuplicationList dupList;
    dupList = findDuplicate(TEST_ENV);
    char *expectedFileName[] = {"brown.txt", "quick.txt"};
    TEST_ASSERT_LIST_FP_NAME(dupList.list, expectedFileName, 2);
}

/** 
 *    
 *        tempFolder                      --------->   subFolder1                   --->   subFolder2
 *       | brown.txt | --- duplicate(1)  |            |quick.txt| -- duplicate(2)  |      |jonSnow.txt|
 *       | subFolder1| -------------------            |missandei.txt|              |      |  olly.txt |
 *       | fox.txt|                                   |khaleesi.txt|               |      | fox.txt | --- duplicate(3)
 *       |jumps.txt|                                  |subFolder2| ----------------
 *                                                    
 */
void test_fileDuplication_given_1File_and_otherFile_in_otherFolder_duplicate(void){
    char subFolder1[256];
    char subFolder2[256];
    sprintf(subFolder1, "%s/%s", TEST_ENV, "subFolder1");
    sprintf(subFolder2, "%s/%s", subFolder1, "subFolder2");
    mkdir(subFolder1);
    mkdir(subFolder2);

    //create duplicate files
    createFileGivenString(TEST_ENV, "brown.txt", "i am duplicate!");
    createFileGivenString(subFolder1, "quick.txt", "i am duplicate!");
    createFileGivenString(subFolder2, "fox.txt", "i am duplicate!");

    //create placeholder files
    createTempFile(TEST_ENV, "fox.txt", 20);
    createTempFile(TEST_ENV, "jump.txt", 20);

    createTempFile(subFolder1, "missandei.txt", 20);
    createTempFile(subFolder1, "khaleesi.txt", 20);

    createTempFile(subFolder2, "jonsnow.txt", 20);
    createTempFile(subFolder2, "olly.txt", 20);
	
	DuplicationList dupList;
    dupList = findDuplicate(TEST_ENV);

    // listAllDuplication(dupList);

    char *expectedFileName[] = {"brown.txt", "quick.txt", "fox.txt"};
    TEST_ASSERT_LIST_FP_NAME(dupList.list, expectedFileName, 3);
}

/** 
 *    
 *        tempFolder                      --------->   subFolder1                   --->   subFolder2
 *       | brown.txt | --- duplicate(1)  |            |quick.txt| -- duplicate(2)  |      |jonSnow.txt|
 *       | subFolder1| -------------------            |missandei.txt|              |      |  olly.txt |
 *       | fox.txt|                                   |khaleesi.txt|               |      | fox.txt | --- duplicate(3)
 *       |jumps.txt|                                  |subFolder2| ----------------
 *                                                    
 */
void test_fileDuplication_given_2_total_duplication(void){
    char subFolder1[256];
    char subFolder2[256];
    sprintf(subFolder1, "%s/%s", TEST_ENV, "subFolder1");
    sprintf(subFolder2, "%s/%s", subFolder1, "subFolder2");
    mkdir(subFolder1);
    mkdir(subFolder2);

    //create duplicate files
    createFileGivenString(TEST_ENV, "brown.txt", "i am duplicate!");
    createFileGivenString(subFolder1, "quick.txt", "i am duplicate!");
    createFileGivenString(subFolder2, "fox.txt", "i am duplicate!");

    //create placeholder files
    createFileGivenString(TEST_ENV, "fox1.txt", "i am duplicate #2");
    createFileGivenString(TEST_ENV, "jump.txt", "i am duplicate #2");

    createTempFile(subFolder1, "missandei.txt", 20);
    createTempFile(subFolder1, "khaleesi.txt", 20);

    createTempFile(subFolder2, "jonsnow.txt", 20);
    createTempFile(subFolder2, "olly.txt", 20);

    DuplicationList dupList;
    dupList = findDuplicate(TEST_ENV);

    listAllDuplication(dupList);

    char *expectedFileName[] = {"brown.txt", "quick.txt", "fox.txt"};
    char *expectedFileName2[] = {"fox1.txt", "jump.txt"};
    TEST_ASSERT_LIST_FP_NAME(dupList.list, expectedFileName2, 2);
    TEST_ASSERT_LIST_FP_NAME(dupList.list + 1, expectedFileName, 3);
}