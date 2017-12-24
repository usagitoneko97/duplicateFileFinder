#ifndef _FILEHANDLER_H
#define _FILEHANDLER_H

#include <dirent.h>

typedef struct{
    DIR *dr;
    char *path;
} FileObj;
/*
typedef struct{
    char *name;
    long long size;
    Date dateModified;
} FileContent;

typedef struct{
    int year;
    int month;
    int day;
    int hour;
    int minute;
} Date;*/

void updateFileInfo();
void _updateFileInfo(DIR *dr, char* path);
int isRegularFile(const char *path);
FILE *getNextFile(FileObj *fileObj, const char *mode);
void openSrc();

void removeDir(char *path);
void _removeDir(DIR *dr, char *path);
int getSize(FILE *fp);

#endif // _FILEHANDLER_H
