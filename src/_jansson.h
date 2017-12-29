#ifndef __JANSSON_H
#define __JANSSON_H

#include <jansson.h>
int json_object_to_file(char *buffer, json_t *json);
json_t *json_object_from_file(char *buffer);
#endif // __JANSSON_H
