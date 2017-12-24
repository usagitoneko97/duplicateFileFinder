#include <stdio.h>
#include "fileHandler.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <malloc.h>

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

FILE *getNextFile(FileObj *fileObj, const char *mode){
    struct dirent *de;
    DIR *currentDr;
    FILE *fileReturn;
    char *currentDir;
    do{
        if ((de = readdir(fileObj->dr)) != NULL){
            //still has file/folder
            if(!strcmp(de->d_name, ".") || !strcmp(de->d_name, "..")){
                continue;
            }
            char temp[256];
            strcpy(temp, fileObj->path);
            strcat(temp, "/");
            strcat(temp, de->d_name);
            if (isRegularFile(temp)){
                return fopen(temp, mode);
            }
            else{
                //backup the current path
                currentDir = fileObj->path;
                currentDr = fileObj->dr;
                fileObj->dr = opendir(temp);
                fileObj->path = temp;
                fileReturn = getNextFile(fileObj, mode);
                //reload with the upper dir value
                closedir(fileObj->dr);
                fileObj->dr = currentDr;
                fileObj->path = currentDir;
                return fileReturn;
            }
        }
        else{
            break;
        }
    } while (!strcmp(de->d_name, ".") || !strcmp(de->d_name, ".."));
    return NULL;
}

void openSrc(){

    struct dirent *de; // Pointer for directory entry

    // opendir() returns a pointer of DIR type.
    DIR *dr = opendir("test/support");

    if (dr == NULL) // opendir returns NULL if couldn't open directory
    {
        printf("Could not open current directory");
        return;
    }

    while ((de = readdir(dr)) != NULL)
        if (isRegularFile("build/Readme.txt"))
            printf("done!");

    closedir(dr);
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
        if (de->d_name[0] == '.')
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
            strcat(tempPath, "/");
            _removeDir(opendir(tempPath), tempPath);
            rmdir(tempPath);
        }
    }
    return;
}

void updateJson(const char *workingDir){
    FILE *file;
    FileObj fileObj;
    fileObj.dr = opendir(workingDir);
    fileObj.path = (char *)malloc(sizeof(char) * 256);
    strcpy(fileObj.path, workingDir);
    while(file != NULL){
        file = getNextFile(&fileObj, "r");
        //process json here
    }
}