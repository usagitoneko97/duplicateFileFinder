#ifndef _FILEHANDLER_H
#define _FILEHANDLER_H

#include <dirent.h>
#include <jansson.h>
#include "_jansson.h"

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
} FileProperty;

typedef struct
{
    char *name;
} FolderContent;

void updateFileInfo();
void _updateFileInfo(DIR *dr, char* path);
int isRegularFile(const char *path);
FileProperty* getNextFile(FileObj *fileObj);
FolderContent *getNextFolder(FileObj *fileObj);
void openSrc();

void removeDir(char *path);
void _removeDir(DIR *dr, char *path);
int getSize(char *path);

void createJson(char *workingDir);
void updateJson(char *path);

void loadFileObjWithPath(char *workingDir, FileObj *fileObj);
json_t* createJsonObjectOnFolder(FileObj *fileObj);
void createJsonObjectFromFileProp(FileProperty *fp, json_t *fileParentJson);

#endif // _FILEHANDLER_H
