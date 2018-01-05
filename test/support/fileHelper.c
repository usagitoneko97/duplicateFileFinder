#include <string.h>
#include <stdio.h>



void createTempFile(const char *path, const char *name, int size)
{
    char completePath[256];
    strcpy(completePath, path);
    strcat(completePath, "/");
    strcat(completePath, name);

    FILE *filePtr = fopen(completePath, "w");
    //generate file of this size
    fseek(filePtr, size - 1, SEEK_SET);
    fputc('\0', filePtr);

    fclose(filePtr);
}

void removeTempFile(const char *pathName)
{
    remove(pathName);
}