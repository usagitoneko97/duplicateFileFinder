#include <stdio.h>
#include "fileHandler.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <malloc.h>
#include <jansson.h>
#include "_jansson.h"

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

FileContent* getNextFile(FileObj *fileObj)
{
    struct dirent *de;
    DIR *currentDr;
    FileContent *fileReturn = NULL;
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
                fileReturn = (FileContent *)(malloc(sizeof(FileContent)));
                // TODO : member others than name
                fileReturn->name = de->d_name;
                fileReturn->size = getSize(temp);
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
                fileReturn = (FolderContent *)(malloc(sizeof(FileContent)));
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

void updateJson(char *workingDir){
    json_t *fileParentJson;
    json_t *filePropertiesJson;

    char buffer[255];
    fileParentJson = json_object();
    FileContent *file = NULL;
    FolderContent *folder;
    FileObj fileObj;
    fileObj.dr = opendir(workingDir);
    fileObj.path = workingDir;
    
	file = getNextFile(&fileObj);
    while(file != NULL){
        filePropertiesJson = json_object();
        json_object_set_new(filePropertiesJson, "size",
                            json_integer(file->size)); 

        json_object_set_new(fileParentJson, file->name, filePropertiesJson);
        printf(json_dumps(fileParentJson, JSON_INDENT(4)));
        sprintf(buffer, "%s/%s", workingDir, JSON_FILE_NAME);

        file = getNextFile(&fileObj);
    }
    json_object_to_file(buffer, fileParentJson);
    // FIXME json_decref(fileParentJson) will cause malloc after that bad memory access
    // free(fileParentJson);
    // json_decref(filePropertiesJson);


    
    // printf(json_dumps(fileParentJson, JSON_INDENT(4)));

    //reload the dr
    fileObj.dr = opendir(workingDir);
    folder = getNextFolder(&fileObj);
    while(folder != NULL){
		sprintf(buffer, "%s/%s", workingDir, folder->name);
        updateJson(buffer);
        folder = getNextFolder(&fileObj);
    }
    return;
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