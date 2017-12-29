#ifndef _FILEHANDLER_H
#define _FILEHANDLER_H

#include <dirent.h>

#define JSON_FILE_NAME      ".property.json"

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
    //..
} FileContent;

typedef struct
{
    char *name;
} FolderContent;

void updateFileInfo();
void _updateFileInfo(DIR *dr, char* path);
int isRegularFile(const char *path);
FileContent* getNextFile(FileObj *fileObj);
FolderContent *getNextFolder(FileObj *fileObj);
void openSrc();

void removeDir(char *path);
void _removeDir(DIR *dr, char *path);
int getSize(char *path);

void updateJson(char *workingDir);

#endif // _FILEHANDLER_H
