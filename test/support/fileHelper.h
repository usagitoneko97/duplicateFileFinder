
#ifndef __FILEHELPER_H
#define __FILEHELPER_H

char *TEST_ENV = "tempFolder";

void createTempFile(const char *path, const char *name, int size);
void removeTempFile(const char *pathName);

#endif // _FILEHELPER_H