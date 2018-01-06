#ifndef _NODEVERIFIER_H
#define _NODEVERIFIER_H
#include "Node.h"

#define TEST_ASSERT_EQUAL_NODE(node, left,right,bf) \
                          testAssertEqualNode(node, left,right,bf,__LINE__)

void testAssertEqualNode(Node *node, Node *left, Node *right, int bf ,int lineNo);

#endif // _NODEVERIFIER_H
