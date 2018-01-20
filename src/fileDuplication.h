#ifndef _FILEDUPLICATION_H
#define _FILEDUPLICATION_H

#include "json2Avl.h"

typedef struct _Item _Item;
struct _Item
{
    _Item *next;
    FileProperty *data;
};

typedef struct _LinkedList _LinkedList;
struct _LinkedList
{
    _Item *head;
    _Item *tail;
    int len;
};

typedef struct _DuplicationList _DuplicationList;
struct _DuplicationList
{
    int numberOfDuplication;
    _LinkedList *list;
};

_DuplicationList * findDuplicate(const char *path);
void listAllDuplication(DuplicationList dl);
void removeAllFileProperty(const char *path);
DuplicationList *testSwig(const char *path);

#endif // _FILEDUPLICATION_H
