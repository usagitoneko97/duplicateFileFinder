#ifndef _FILEDUPLICATION_H
#define _FILEDUPLICATION_H

#include "json2Avl.h"

DuplicationList findDuplicate(char *path);
void listAllDuplication(DuplicationList dl);

#endif // _FILEDUPLICATION_H
