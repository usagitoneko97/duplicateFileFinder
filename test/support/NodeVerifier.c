#include "NodeVerifier.h"
#include "Node.h"
#include "unity.h"
#include <stdarg.h>
#include <stdio.h>
#include <malloc.h>


char *createMessage(char *message, ...){
  va_list args;
  char *buffer;
  int length;

  va_start(args,message);

  length = vsnprintf(buffer, 0, message, args);
  buffer = malloc(length+1);
  vsnprintf(buffer, length+1, message, args);

  return buffer;
}

void testAssertEqualNode(Node *node, Node *left, Node *right, int balanceFactor, int lineNo)
{
  char *error;
  if(left != NULL){
    if(node->left != left){
      error = createMessage("Expected left node to be 0x%p, but was %p",  \
                            left,node->left);
      UNITY_TEST_FAIL(lineNo,error);
    }
  }
  else{
    if(node->left != NULL){
      error = createMessage("Expected left node to be NULL, but was %p",  \
                            node->left);
      UNITY_TEST_FAIL(lineNo,error);
    }
  }
  if(right != NULL){
    if(node->right != right){
      error = createMessage("Expected right node to be 0x%p, but was %p",  \
                            right,node->right);
    UNITY_TEST_FAIL(lineNo,error);
    }
  }
  else{
    if(node->right != NULL){
      error = createMessage("Expected right node to be NULL, but was %p",  \
                            node->right);
      UNITY_TEST_FAIL(lineNo,error);
    }
  }
  if(balanceFactor != node->balanceFactor){
    error = createMessage("Expected balance factor to be %d, but was %d",  \
                            balanceFactor, node->balanceFactor);
    UNITY_TEST_FAIL(lineNo,error);
  }


}

