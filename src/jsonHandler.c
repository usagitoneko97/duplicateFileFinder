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
    closedir(fileObj.dr);
}

/** 
 * @brief  compare the date structure
 * @param  date1: first date
 *         json: second date contain in json object
 * @retval 1: if two of the date compared is equal
 *         0: if the date is not equal
 */
int compareDate(Date date1, json_t *json){
    Date realDate;
    realDate.year = json_integer_value(json_object_get(json, "year"));
    realDate.month = json_integer_value(json_object_get(json, "month"));
    realDate.day = json_integer_value(json_object_get(json, "day"));
    realDate.hour = json_integer_value(json_object_get(json, "hour"));
    realDate.minute = json_integer_value(json_object_get(json, "minute"));
    realDate.seconds = json_integer_value(json_object_get(json, "seconds"));

    if (date1.year != realDate.year ||
        date1.month != realDate.month ||
        date1.day != realDate.day ||
        date1.hour != realDate.hour ||
        date1.minute != realDate.minute ||
        date1.seconds != realDate.seconds)
    {
       return 0;
    }
    return 1;
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

    json_t *jsonFile;
    //get next file
    //check from .property.json
    //if no then add into it


    //insert file into .property.json
    while ((file = getnextFileIgnoreProperty(&fileObj)) != NULL)
    {
        if ((jsonFile = json_object_get(json, file->name)) != NULL){
            json_t *jsonDate = json_object_get(jsonFile, "dateModified");
            if (compareDate(file->dateModified, jsonDate) == 1){
                // file obtained and data inside propertyJson are equal
                continue;
            }
        }
        //update the json object with newly obtained file property
        if (json == NULL)
        {
            //property.json is empty
            json = json_object();
        }
        createJsonObjectFromFileProp(file, json);
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
    json_t *timeJson;
    filePropertiesJson = json_object();
    json_object_set_new(filePropertiesJson, "size",
                        json_integer(fp->size));

    json_object_set_new(filePropertiesJson, "crc",
                        json_integer(fp->crc));

    timeJson = json_pack("{sisisisisisi}",
                         "year", fp->dateModified.year,
                         "month", fp->dateModified.month,
                         "day", fp->dateModified.day,
                         "hour", fp->dateModified.hour,
                         "minute", fp->dateModified.minute,
                         "seconds", fp->dateModified.seconds);

    json_object_set_new(filePropertiesJson, "dateModified", timeJson);

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

void updateCreateAllJsonOnFolder(char *path)
{
    //check if property json exist
    char buffer[256];
    char propertyJsonPath[256];
    FolderContent *folder;
    sprintf(propertyJsonPath, "%s/%s", path, JSON_FILE_NAME);
    FILE *propertyJsonFile = fopen(propertyJsonPath, "r");
    if(propertyJsonFile == NULL){
        //createJson
        createJson(path);
    }
    else{
        //exist, update Json
		fclose(propertyJsonFile);
		updateJson(path);
		
    }

    //recursive to next folder

    FileObj fileObj;
    loadFileObjWithPath(path, &fileObj);

    folder = getNextFolder(&fileObj);
    while (folder != NULL)
    {
        sprintf(buffer, "%s/%s", path, folder->name);
        updateCreateAllJsonOnFolder(buffer);
        free(folder);
        folder = getNextFolder(&fileObj);
    }
    fileObj.dr = opendir(path);
    closedir(fileObj.dr);
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
