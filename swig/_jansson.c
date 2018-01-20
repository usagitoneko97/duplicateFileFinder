#include "_jansson.h"
#include <stdio.h>

int json_object_to_file(char *buffer, json_t *json){
    FILE *fileP = fopen(buffer, "w");
    int status = json_dumpf(json, fileP, JSON_INDENT(4));
    fclose(fileP);
    return status;
}

json_t* json_object_from_file(char *buffer){
    json_error_t error;
    FILE *file = fopen(buffer, "r");
    json_t *obj = json_loadf(file, JSON_DECODE_ANY, &error);
    fclose(file);
    return obj;
}