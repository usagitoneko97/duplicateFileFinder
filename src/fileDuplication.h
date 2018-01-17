#ifndef _FILEDUPLICATION_H
#define _FILEDUPLICATION_H

#include "json2Avl.h"

DuplicationList findDuplicate(char *path);
void listAllDuplication(DuplicationList dl);
int assertPath(char *pathArg, char *resultPath);
void removeAllFileProperty(char *path);

#endif // _FILEDUPLICATION_H
