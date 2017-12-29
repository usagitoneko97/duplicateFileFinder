#ifndef _FILEHANDLER_H
#define _FILEHANDLER_H

#include <dirent.h>

typedef struct{
    DIR *dr;
    char *path;
} FileObj;

typedef struct
{
    int year;
    int month;
    int day;
    int hour;
    int minute;
} Date;

typedef struct{
    char *name;
    long long size;
    Date dateModified;
} FileContent;


void updateFileInfo();
void _updateFileInfo(DIR *dr, char* path);
int isRegularFile(const char *path);
FileContent* getNextFile(FileObj *fileObj, const char *mode);
void openSrc();

void removeDir(char *path);
void _removeDir(DIR *dr, char *path);
int getSize(FILE *fp);

#endif // _FILEHANDLER_H
