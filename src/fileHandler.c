#include <stdio.h>
#include "fileHandler.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <malloc.h>
#include "crc.h"
#include <time.h>


int isRegularFile(const char *path)
{
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
        return 0;
    return !S_ISDIR(statbuf.st_mode);
}

void updateFileInfo(){
    DIR *dr = opendir(".");
    char path[256] = "";
    _updateFileInfo(dr, path);
}

void _updateFileInfo(DIR *dr, char* path){
    struct dirent *de;
    char tempPath[256];
    while ((de = readdir(dr)) != NULL){
		strcpy(tempPath, path);
        if (de->d_name[0] == '.')
        {
            continue;
        }
        strcat(tempPath, de->d_name);
        if (isRegularFile(tempPath))
            printf("%s\n", de->d_name);
        else{
                //printf("%s\n", de->d_name);
				strcat(tempPath, "/");
                _updateFileInfo(opendir(tempPath), tempPath);
            }
        
    }
    return;
}

FileProperty* getNextFile(FileObj *fileObj)
{
    struct dirent *de;
    DIR *currentDr;
    FileProperty *fileReturn = NULL;
    char *currentDir;
    do{
        if ((de = readdir(fileObj->dr)) != NULL){
            
            //still has file/folder
            if(!strcmp(de->d_name, ".") || !strcmp(de->d_name, "..")){
                continue;
            }
            char temp[256];
            sprintf(temp, "%s/%s", fileObj->path, de->d_name);
            if (isRegularFile(temp)){
                fileReturn = (FileProperty *)(malloc(sizeof(FileProperty)));
                // TODO : member others than name
                fileReturn->name = de->d_name;
                fileReturn->size = getSize(temp);
                fileReturn->crc = getCrcGivenPath(temp);
                fileReturn->dateModified = getFileModifiedDate(temp);
                return fileReturn;
            }
            else{
                continue;
            }
        }
        else{
            return NULL;
        }
    } while (!strcmp(de->d_name, ".") || !strcmp(de->d_name, "..") || fileReturn == NULL);
    return NULL;
}

Date getFileModifiedDate(char *path){
    struct tm *foo;
    struct stat attrib;
    stat(path, &attrib);
    foo = localtime(&attrib.st_mtime);
    Date date;
    date.year = foo->tm_year + 1900;
    date.month = foo->tm_mon + 1;
    date.day = foo->tm_mday;
    date.hour = foo->tm_hour;
    date.minute = foo->tm_min;
    date.seconds = foo->tm_sec;
    return date;
}

char *readContentOfGivenPath(char *path){
    FILE *pFile;
    long lSize;
    char *buffer;
    size_t result;

    pFile = fopen(path, "rb");
    if (pFile == NULL)
    {
        return NULL;
    }

    // obtain file size:
    lSize = getSize(path);

    // allocate memory to contain the whole file:
    buffer = (char *)malloc(sizeof(char) * lSize);
    if (buffer == NULL)
    {
        return NULL;
    }

    // copy the file into the buffer:
    result = fread(buffer, 1, lSize, pFile);
    if (result != lSize)
    {
        fputs("Reading error", stderr);
        exit(3);
    }

    /* the whole file is now loaded in the memory buffer. */

    // terminate
    fclose(pFile);
    return buffer;
}

FileProperty *getnextFileIgnoreProperty(FileObj *fileObj){
    FileProperty *temp;
    temp = getNextFile(fileObj);
    if(temp == NULL){
        return NULL;
    }
    if(!strcmp(temp->name, ".property.json")){
        free(temp);
        return getNextFile(fileObj);
    }
    return temp;
}


FolderContent *getNextFolder(FileObj *fileObj)
{
    struct dirent *de;
    DIR *currentDr;
    FolderContent *fileReturn = NULL;
    char *currentDir;
    do
    {
        if ((de = readdir(fileObj->dr)) != NULL)
        {

            //still has file/folder
            if (!strcmp(de->d_name, ".") || !strcmp(de->d_name, ".."))
            {
                continue;
            }
            char temp[256];
            sprintf(temp, "%s/%s", fileObj->path, de->d_name);
            if (isRegularFile(temp))
            {
                continue;
            }
            else
            {
                fileReturn = (FolderContent *)(malloc(sizeof(FileProperty)));
                // TODO : member others than name
                fileReturn->name = de->d_name;

                return fileReturn;
                
            }
        }
        else
        {
            return NULL;
        }
    } while (!strcmp(de->d_name, ".") || !strcmp(de->d_name, "..") || fileReturn == NULL);
    return NULL;
}


void removeDir(char *path)
{
    DIR *dr = opendir(path);
    if(dr == NULL)
        return;
    _removeDir(dr, path);
    closedir(dr);
    rmdir(path);
}

void _removeDir(DIR *dr, char *path)
{
    struct dirent *de;
    char tempPath[256];
    while ((de = readdir(dr)) != NULL)
    {
        strcpy(tempPath, path);
        if (!strcmp(de->d_name, ".") || !strcmp(de->d_name, ".."))
        {
            continue;
        }
        strcat(tempPath, "/");
        strcat(tempPath, de->d_name);
        if (isRegularFile(tempPath)){
            //delete file
            remove(tempPath);
        }
        else
        {
            //printf("%s\n", de->d_name);
            _removeDir(opendir(tempPath), tempPath);
            rmdir(tempPath);
        }
    }
    return;
}


void loadFileObjWithPath(char *workingDir, FileObj *fileObj){
    fileObj->dr = opendir(workingDir);
    fileObj->path = workingDir;
}



int getSize(char *path){
    FILE *fp = fopen(path, "r");
    int sz;
    fseek(fp, 0L, SEEK_END);
    sz = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    fclose(fp);
    return sz;
}

