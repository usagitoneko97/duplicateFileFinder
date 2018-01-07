#ifndef _NODEVERIFIER_H
#define _NODEVERIFIER_H

#include "Node.h"
#include "AvlInteger.h"

#define TEST_ASSERT_EQUAL_NODE(node, left,right,balanceFactor) \
                  testAssertEqualNode((Node*)node, (Node*)left,(Node*)right,balanceFactor,__LINE__)


void testAssertEqualNode(Node *node, Node *left, Node *right, int balanceFactor, int lineNo);

#endif // _NODEVERIFIER_H
