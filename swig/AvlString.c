#include "AvlString.h"
#include <string.h>
#include "stdio.h"


int compareStrVoidPtr(void *str1, Node *str2)
{
  char *strNode1 = (char *)str1;
  StrNode *strNode2 = (StrNode *)str2;
  int result = strcmp(strNode1, strNode2->data);
  if (result > 0)
  {
    return 1;
  }
  else if (result < 0)
  {
    return -1;
  }
  else
  {
    return 0;
  }
}
