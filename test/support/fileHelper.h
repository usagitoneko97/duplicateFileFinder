
#ifndef __FILEHELPER_H
#define __FILEHELPER_H

#include <jansson.h>
#include "fileHandler.h"
#include "linkedlist.h"

#define TEST_ENV    "tempFolder"

#define TEST_ASSERT_JSON_PROPERTY(json, fp, length)         testAssertJson(json, fp, length, __LINE__)
#define TEST_ASSERT_JSON_PROPERTY_PATH(path, fp, length)    testAssertJsonPath(path, fp, length, __LINE__)
#define TEST_ASSERT_EQUAL_DATE(date1, date2)                testAssertEqualDate(date1, date2, __LINE__)

#define TEST_ASSERT_LIST_FP(list, fp, length) testAssertListWithFp(list, fp, length, __LINE__)

FileProperty createTempFile(const char *path, const char *name, int size);
FileProperty createTempFileWithDate(const char *path, const char *name, int size, Date date);

void removeTempFile(const char *pathName);
void testAssertJson(json_t *json, FileProperty *fp, int length, int lineNo);
void testAssertJsonPath(char *path, FileProperty *fp, int length, int lineNo);
void testAssertEqualDate(Date date1, Date date2, int lineNo);
void testAssertListWithFp(LinkedList *list, FileProperty *fp, int length, int lineNo);

void createJsonFileFromFp(char *path, FileProperty *fp, int length);
void printDate(Date date);
void setFileModifiedDate(char *path, Date date);
int compareFp(FileProperty *fp1, FileProperty *fp2);
int compare2Date(Date date1, Date date2);

#endif // _FILEHELPER_H