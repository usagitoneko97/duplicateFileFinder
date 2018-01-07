#include "unity.h"
#include "AddAvl.h"
#include "NodeHelper.h"
#include "NodeVerifier.h"
#include "rotate.h"
#include "Node.h"
#include "Exception.h"
#include "CException.h"
#include "AvlInteger.h"
#include "AvlString.h"
#include "DeleteAvl.h"
CEXCEPTION_T ex;
int heightChanged;

#define initNodeInt(node, left, right, bf)  initNode((Node*)node, (Node*)left, (Node*)right, bf);

void setUp(void)
{
  initIntegerNodeData();
}

void tearDown(void)
{
}

/**
 *    NULL -->  node1
 */
void testavlAddInteger_given_NULL_add_1Node(void)
{
  IntegerNode *root = NULL;
  Try{
    avlAddInteger(&root, &node5);
  }Catch(ex){
    dumpException(ex);
  }

  TEST_ASSERT_EQUAL_NODE(&node1, NULL, NULL, 0);
}

/**
 *    node1  ---->   node1
 *                      \
 *                      node5
 */
void testavlAddInteger_given_node1_add_node5(void){
  initNodeInt(&node1, NULL, NULL, 0);
  initNodeInt(&node5, &node5, &node5, 0);

  IntegerNode *root = &node1;
  Node *rootNode = (Node *)root;
  Try{
    avlAddInteger(&rootNode, &node5);
  }Catch(ex){
    dumpException(ex);
  }
  TEST_ASSERT_EQUAL_NODE(&node1, NULL, &node5, 1);
  TEST_ASSERT_EQUAL_NODE(&node5, NULL, NULL, 0);
}

/**
 *    node5  ---->   node5
 *                  /
 *                node1
 */
void testavlAddInteger_given_node5_add_node1(void){
  initNodeInt(&node1, NULL, NULL, 0);
  initNodeInt(&node5, NULL, NULL, 0);

  IntegerNode *root = &node5;
  Try{
    avlAddInteger(&root, &node1);
  }Catch(ex){
    dumpException(ex);
  }
  TEST_ASSERT_EQUAL_NODE(&node5, &node1, NULL, -1);
  TEST_ASSERT_EQUAL_NODE(&node1, NULL, NULL, 0);
}

/**
 *     10(-1)   (add 20)        10 (0)
 *    /          ----->        /  \
 *   5                        5    20
 */
void testavlAddInteger_given_10_5_add_20(void){
  initNodeInt(&node10, &node5, NULL,-1);
  initNodeInt(&node5, NULL, NULL, 0);
  initNodeInt(&node20, NULL, NULL, 0);

  IntegerNode *root = &node10;
  Try{
    avlAddInteger(&root, &node20);
  }Catch(ex){
    dumpException(ex);
  }

  TEST_ASSERT_EQUAL_NODE(&node10, &node5, &node20, 0);
  TEST_ASSERT_EQUAL_NODE(&node1, NULL, NULL, 0);
}

/**
 *    node1                  node1                     node5 (0)
 *        \         ----->     \            ---->      /    \
 *        node5                 node5               node1    node10
 *                               \
 *                               node10
 */
void testavlAddInteger_given_node1_node5_add_node10(void){
  initNodeInt(&node1, NULL, &node5, 1);
  initNodeInt(&node5, NULL, NULL, 0);
  initNodeInt(&node10, NULL, NULL, 0);

  IntegerNode *root = &node1;
  Try{
    heightChanged = avlAddInteger(&root, &node10);
  }Catch(ex){
    dumpException(ex);
  }
  TEST_ASSERT_EQUAL(NO_CHANGED, heightChanged);
  TEST_ASSERT_EQUAL_NODE(&node5, &node1 , &node10, 0);
  TEST_ASSERT_EQUAL_NODE(&node1, NULL, NULL, 0);
  TEST_ASSERT_EQUAL_NODE(&node10, NULL, NULL, 0);

}

/**
 *        node10 (1)          node10 (-2)             node5(0)
 *        /          --->    /              -->      /    \
 *     node5               node5  (-1)            node1  node10
 *                         /
 *                        node1  (0)
 */
void testavlAddInteger_given_node10_node5_add_node1(void){
  initNodeInt(&node10, &node5, NULL, -1);

  initNodeInt(&node5, NULL, NULL, 0);
  initNodeInt(&node1, &node1, &node1, 0);

  IntegerNode *root = &node10;
  Try{
    heightChanged = avlAddInteger(&root, &node1);
  }Catch(ex){
    dumpException(ex);
  }
  TEST_ASSERT_EQUAL(NO_CHANGED, heightChanged);
  TEST_ASSERT_EQUAL_NODE(&node5, &node1, &node10, 0);
  TEST_ASSERT_EQUAL_NODE(&node10, NULL , NULL, 0);
  TEST_ASSERT_EQUAL_NODE(&node1, NULL , NULL, 0);
}

/**
 *     node15               node15(-2)                node10
 *     /    (add node10)    /          (rotate LR)    /   \
 *   node5      --->       node5(1)       ---->   node5   node15
 *                             \
 *                             node10
 */
void testavlAddInteger_given_node15_node5_add_node10(void){
  initNodeInt(&node15, &node5, NULL, -1);
  initNodeInt(&node5, NULL, NULL, 0);
  initNodeInt(&node10, NULL, NULL, 0);

  IntegerNode *root = &node15;
  Try{
    heightChanged = avlAddInteger(&root, &node10);
  }Catch(ex){
    dumpException(ex);
  }
  TEST_ASSERT_EQUAL(NO_CHANGED, heightChanged);
  TEST_ASSERT_EQUAL_NODE(&node10, &node5, &node15, 0);
  TEST_ASSERT_EQUAL_NODE(&node5, NULL , NULL, 0);
  TEST_ASSERT_EQUAL_NODE(&node15, NULL , NULL, 0);
}

/**
 *     node5        (add node10)     node5      (rotate RL)   node10
 *         \           --->              \         --->       /    \
 *         node15                        node15            node5   node15
 *                                       /
 *                                     node10
 *
 *
 */
void testavlAddInteger_given_node5_node15_add_node10_expect_rotateRL(void){
  initNodeInt(&node5, NULL, &node15, 1);
  initNodeInt(&node15, NULL, NULL, 0);
  initNodeInt(&node10, NULL, NULL, 0);

  IntegerNode *root = &node5;
  Try{
    heightChanged = avlAddInteger(&root, &node10);
  }Catch(ex){
    dumpException(ex);
  }

  TEST_ASSERT_EQUAL(NO_CHANGED, heightChanged);
  TEST_ASSERT_EQUAL_NODE(&node10, &node5, &node15, 0);
  TEST_ASSERT_EQUAL_NODE(&node5, NULL , NULL, 0);
  TEST_ASSERT_EQUAL_NODE(&node15, NULL , NULL, 0);
}

/**
*      node5                      5
*     /     \                   /  \
*   node1   node15     --->    1   15
*             \                    / \
*             node20              10  20
*
 */
void testavlAddInteger_parent_noHeightChanged_add_10(void){
  initNodeInt(&node5, &node1, &node15, 1);
  initNodeInt(&node15, NULL, &node20, 1);
  initNodeInt(&node1, NULL, NULL, 0);
  initNodeInt(&node20, NULL, NULL, 0);

  initNodeInt(&node10, NULL, NULL, 0);

  IntegerNode *root = &node5;
  Try{
    heightChanged = avlAddInteger(&root, &node10);
  }Catch(ex){
    dumpException(ex);
  }
  TEST_ASSERT_EQUAL(NO_CHANGED, heightChanged);
  TEST_ASSERT_EQUAL_NODE(&node5, &node1, &node15, 1);
  TEST_ASSERT_EQUAL_NODE(&node15, &node10, &node20, 0);

  //TODO change balanceFactor after rotate
  TEST_ASSERT_EQUAL_NODE(&node1, NULL, NULL, 0);
  TEST_ASSERT_EQUAL_NODE(&node10, NULL, NULL, 0);
  TEST_ASSERT_EQUAL_NODE(&node20, NULL, NULL, 0);
}

/*
 *
 *      node5                           node5                     node5
 *     /     \                          /   \                     /    \
 *   node1   node10   (add node15 )  node1  node10 (rotate RL)  node1 node15
 *             \      --->                      \      --->           /   \
 *             node20                          node20             node10  node20
 *                                             /
 *                                           node15
 *
 */
void testavlAddInteger_given_above_expect_rotateRL(void){
  initNodeInt(&node5, &node1, &node10, 1);
  initNodeInt(&node10, NULL, &node20, 1);
  initNodeInt(&node1, NULL, NULL, 0);
  initNodeInt(&node20, NULL, NULL, 0);
  initNodeInt(&node15, NULL, NULL, 0);

  IntegerNode *root = &node5;
  Try{
    heightChanged = avlAddInteger(&root, &node15);
  }Catch(ex){
    dumpException(ex);
  }
  TEST_ASSERT_EQUAL(NO_CHANGED, heightChanged);
  TEST_ASSERT_EQUAL_NODE(&node5, &node1, &node15, 1);
  TEST_ASSERT_EQUAL_NODE(&node15, &node10 , &node20, 0);

  //TODO change balanceFactor after rotate
  TEST_ASSERT_EQUAL_NODE(&node1, NULL , NULL, 0);
  TEST_ASSERT_EQUAL_NODE(&node10, NULL , NULL, 0);
  TEST_ASSERT_EQUAL_NODE(&node20, NULL , NULL, 0);
}

/*
 *
 *         node3(1)                                node3(1)                           node3(1)
 *        /    \                                   /    \                            /       \
 *   node2(-1) node5(1)                        node2   node5                      node2    node5
 *    /       /     \                           /      /   \                       /      /    \
 * node1   node4   node10   (add node15 )  node1    node4  node10 (rotate RL)  node1  node4   node15
 *                    \          --->                          \      --->                    /   \
 *                    node20                                  node20                      node10  node20
 *                                                            /
 *                                                          node15
 *
 */
void testavlAddInteger_given_above_expect_rotateRL__with_2_parents(void){
  initNodeInt(&node3, &node2, &node5, 1);
  initNodeInt(&node2, &node1, NULL, -1);
  initNodeInt(&node5, &node4, &node10, 1);
  initNodeInt(&node10, NULL, &node20, 1);
  initNodeInt(&node1, NULL, NULL, 0);
  initNodeInt(&node20, NULL, NULL, 0);
  initNodeInt(&node4, NULL, NULL, 0);

  initNodeInt(&node15, NULL, NULL, 0);

  IntegerNode *root = &node3;
  Try{
    heightChanged = avlAddInteger(&root, &node15);
  }Catch(ex){
    dumpException(ex);
  }
  TEST_ASSERT_EQUAL(NO_CHANGED, heightChanged);
  TEST_ASSERT_EQUAL_NODE(&node3, &node2, &node5, 1);
  TEST_ASSERT_EQUAL_NODE(&node5, &node4, &node15, 1);
  TEST_ASSERT_EQUAL_NODE(&node15, &node10 , &node20, 0);

  TEST_ASSERT_EQUAL_NODE(&node1, NULL , NULL, 0);
  TEST_ASSERT_EQUAL_NODE(&node10, NULL , NULL, 0);
  TEST_ASSERT_EQUAL_NODE(&node20, NULL , NULL, 0);
}

/**
 *     node5
 *   /     \
 * node1   node10  ---> add node10  expect throw exception
 *           \
 *          node20
 */
void testavlAddInteger_withExistingValue(void){
  initNodeInt(&node5, &node1, &node10, 1);
  initNodeInt(&node10, NULL, &node20, 1);
  initNodeInt(&node1, NULL, NULL, 0);
  initNodeInt(&node20, NULL, NULL, 0);
  initNodeInt(&node15, NULL, NULL, 0);
  IntegerNode *root = &node5;
  Try
  {
    heightChanged = avlAddInteger(&root, &node10);
    TEST_FAIL_MESSAGE("same value added to avlTree but no exception thrown");
  }
  Catch(ex)
  {}
}
