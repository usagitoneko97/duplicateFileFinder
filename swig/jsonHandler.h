#ifndef _JSONHANDLER_H
#define _JSONHANDLER_H

#include "fileHandler.h"

void createJson(char *workingDir);
void updateJson(char *path);

json_t *createJsonObjectOnFolder(FileObj *fileObj);
void createJsonObjectFromFileProp(FileProperty *fp, json_t *fileParentJson);

void updateCreateAllJsonOnFolder(char *path);
int compareDate(Date date1, json_t *json);
#endif // _JSONHANDLER_H
