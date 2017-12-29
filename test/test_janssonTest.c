#include "unity.h"
#include "janssonTest.h"
#include <jansson.h>

void setUp(void)
{
}

void tearDown(void)
{
}

void test_janssonTest_NeedToImplement(void)
{
    json_t *json = json_object();
    json_object_set_new(json, "name", json_string("akai koen"));
    printf("%s \n", json_dumps(json, JSON_INDENT(4)));
}
