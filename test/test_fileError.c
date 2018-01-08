#include "unity.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <malloc.h>

#include <stdarg.h>
void setUp(void)
{
    //create working tst folder
}

void tearDown(void)
{
    //remove test folder after every test
}


void test_fileError(void){
    char *path = "src";
    DIR *dr;
    dr = opendir(path);
    dr = opendir(path);

    closedir(dr);
    // closedir(dr);
    dr = opendir(path);
}
