
#ifndef __FILEHELPER_H
#define __FILEHELPER_H

#include <jansson.h>
#include "fileHandler.h"

#define TEST_ENV    "tempFolder"

#define TEST_ASSERT_JSON_PROPERTY(json, fp, length)         testAssertJson(json, fp, length, __LINE__)
#define TEST_ASSERT_JSON_PROPERTY_PATH(path, fp, length)    testAssertJsonPath(path, fp, length, __LINE__)

FileProperty createTempFile(const char *path, const char *name, int size);
void removeTempFile(const char *pathName);
void testAssertJson(json_t *json, FileProperty *fp, int length, int lineNo);
void testAssertJsonPath(char *path, FileProperty *fp, int length, int lineNo);
void createJsonFileFromFp(char *path, FileProperty *fp, int length);
void printDate(Date date);

#endif // _FILEHELPER_H