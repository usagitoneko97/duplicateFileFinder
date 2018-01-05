#include "jsonHandler.h"


void createJson(char *workingDir)
{
    json_t *fileParentJson;
    char buffer[255];
    FolderContent *folder;
    FileObj fileObj;
    loadFileObjWithPath(workingDir, &fileObj);

    fileParentJson = createJsonObjectOnFolder(&fileObj);
    sprintf(buffer, "%s/%s", workingDir, JSON_FILE_NAME);
    json_object_to_file(buffer, fileParentJson);

    // FIXME json_decref(fileParentJson) will cause malloc after that bad memory access
    // free(fileParentJson);
    // json_decref(filePropertiesJson);

    // printf(json_dumps(fileParentJson, JSON_INDENT(4)));
}

/** 
 * @brief  only called this func when .property.json exist
 *         compare getNextFile with .property.json
 * @note   
 * @param  *path: path to the folder
 * @retval None
 */
void updateJson(char *path)
{
    FileObj fileObj;
    FileProperty *file = NULL;
    loadFileObjWithPath(path, &fileObj);
    char buffer[256];
    json_t *json = json_object();
    sprintf(buffer, "%s/%s", path, JSON_FILE_NAME);
    json = json_object_from_file(buffer);

    //get next file
    //check from .property.json
    //if no then add into it

    file = getnextFileIgnoreProperty(&fileObj);

    //insert file into .property.json
    while (file != NULL)
    {
        if (json_object_get(json, file->name) == NULL)
        {
            //its not inside
            if (json == NULL)
            {
                //property.json is empty
                json = json_object();
            }
            createJsonObjectFromFileProp(file, json);
        }
        file = getnextFileIgnoreProperty(&fileObj);
    }

    //check for object in .property.json exist in file
    const char *key;
    json_t *value;
    void *temp;
    FILE *checkFile;

    json_object_foreach_safe(json, temp, key, value)
    {
        sprintf(buffer, "%s/%s", path, key);
        checkFile = fopen(buffer, "r");
        if (checkFile == NULL)
        {
            //file not exist
            json_object_del(json, key);
        }
        else
        {
            fclose(checkFile);
        }
    }

    if (closedir(fileObj.dr) == -1)
    {
        //error
        printf("error occurs on closedir");
    }
    // if()
    sprintf(buffer, "%s/%s", path, JSON_FILE_NAME);
    json_object_to_file(buffer, json);

    json_decref(json);
}

void createJsonObjectFromFileProp(FileProperty *fp, json_t *fileParentJson)
{
    json_t *filePropertiesJson;

    filePropertiesJson = json_object();
    json_object_set_new(filePropertiesJson, "size",
                        json_integer(fp->size));

    json_object_set_new(fileParentJson, fp->name, filePropertiesJson);
}

json_t *createJsonObjectOnFolder(FileObj *fileObj)
{
    json_t *fileParentJson;
    fileParentJson = json_object();
    FileProperty *file = NULL;

    file = getNextFile(fileObj);
    while (file != NULL)
    {
        createJsonObjectFromFileProp(file, fileParentJson);
        free(file);
        file = getNextFile(fileObj);
    }
    return fileParentJson;
}
/*
void createJson(char *workingDir){
    json_t *fileParentJson;
    char buffer[255];
    FolderContent *folder;
    FileObj fileObj;
    loadFileObjWithPath(workingDir, &fileObj);

    fileParentJson = createJsonObjectOnFolder(&fileObj);
    sprintf(buffer, "%s/%s", workingDir, JSON_FILE_NAME);
    json_object_to_file(buffer, fileParentJson);
    
    // FIXME json_decref(fileParentJson) will cause malloc after that bad memory access
    // free(fileParentJson);
    // json_decref(filePropertiesJson);

    // printf(json_dumps(fileParentJson, JSON_INDENT(4)));
    //reload the dr
    closedir(fileObj.dr);
    fileObj.dr = opendir(workingDir);
    folder = getNextFolder(&fileObj);
    while(folder != NULL){
		sprintf(buffer, "%s/%s", workingDir, folder->name);
        createJson(buffer);
        free(folder);
        folder = getNextFolder(&fileObj);
    }
    fileObj.dr = opendir(workingDir);
    closedir(fileObj.dr);
    return;
}*/
