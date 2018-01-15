#include "unity.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <malloc.h>

#include <stdarg.h>

#include <rhash.h>
#include <crc.h>
#include "fileHandler.h"
void setUp(void)
{
    //create working tst folder
}

void tearDown(void)
{
    //remove test folder after every test
}

void test_rhash(void){
    uint32_t crc = getCrcGivenPath("temp1/test.txt");
    TEST_ASSERT_GREATER_THAN(0, crc);
}
