#ifndef _JSONHANDLER_H
#define _JSONHANDLER_H

#include "fileHandler.h"

void createJson(char *workingDir);
void updateJson(char *path);

json_t *createJsonObjectOnFolder(FileObj *fileObj);
void createJsonObjectFromFileProp(FileProperty *fp, json_t *fileParentJson);
#endif // _JSONHANDLER_H
