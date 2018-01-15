#ifndef _FILEDUPLICATION_H
#define _FILEDUPLICATION_H

#include "json2Avl.h"

DuplicationList findDuplicate(char *path);
void listAllDuplication(DuplicationList dl);
int assertPath(char *pathArg, char *resultPath);

#endif // _FILEDUPLICATION_H
