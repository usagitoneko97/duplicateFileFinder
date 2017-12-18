#ifndef _FILEHANDLER_H
#define _FILEHANDLER_H

#include <dirent.h>

typedef struct{
    DIR *dr;
    char *path;
} FileObj;

void updateFileInfo();
void _updateFileInfo(DIR *dr, char* path);
int isRegularFile(const char *path);
FILE *getNextFile(FileObj *fileObj, const char *mode);
void openSrc();

#endif // _FILEHANDLER_H
