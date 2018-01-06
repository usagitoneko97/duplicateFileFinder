#include "unity.h"
#include "AddAvl.h"
#include "NodeHelper.h"
#include "NodeVerifier.h"
#include "rotate.h"
#include "Node.h"
void setUp(void)
{
  initNodeData();
}

void tearDown(void)
{
}

/**
 *    NULL -->  node1
 */
void test_AddAvl_given_NULL_add_1Node(void)
{
  Node *root = NULL;
  addAvl(&root, &node5);

  TEST_ASSERT_EQUAL_NODE(&node1, NULL, NULL, 0);
}

/**
 *    node1  ---->   node1
 *                      \
 *                      node5
 */
void test_AddAvl_given_node1_add_node5(void){
  initNode(&node1, NULL, NULL, 0);
  initNode(&node5, &node5, &node5, 0);

  Node *root = &node1;
  addAvl(&root, &node5);
  TEST_ASSERT_EQUAL_NODE(&node1, NULL, &node5, 1);
  TEST_ASSERT_EQUAL_NODE(&node5, NULL, NULL, 0);
}

/**
 *    node5  ---->   node5
 *                  /
 *                node1
 */
void test_AddAvl_given_node5_add_node1(void){
  initNode(&node1, NULL, NULL, 0);
  initNode(&node5, NULL, NULL, 0);

  Node *root = &node5;
  addAvl(&root, &node1);
  TEST_ASSERT_EQUAL_NODE(&node5, &node1, NULL, -1);
  TEST_ASSERT_EQUAL_NODE(&node1, NULL, NULL, 0);
}

/**
 *     10(-1)   (add 20)        10 (0)
 *    /          ----->        /  \
 *   5                        5    20
 */
void test_addAvl_given_10_5_add_20(void){
  initNode(&node10, &node5, NULL,-1);
  initNode(&node5, NULL, NULL, 0);
  initNode(&node20, NULL, NULL, 0);

  Node *root = &node10;
  addAvl(&root, &node20);

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
void test_AddAvl_given_node1_node5_add_node10(void){
  initNode(&node1, NULL, &node5, 1);
  initNode(&node5, NULL, NULL, 0);
  initNode(&node10, NULL, NULL, 0);

  Node *root = &node1;
  int heightChanged = addAvl(&root, &node10);
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
void test_AddAvl_given_node10_node5_add_node1(void){
  initNode(&node10, &node5, NULL, -1);

  initNode(&node5, NULL, NULL, 0);
  initNode(&node1, &node1, &node1, 0);

  Node *root = &node10;
  int heightChanged = addAvl(&root, &node1);
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
void test_addAvl_given_node15_node5_add_node10(void){
  initNode(&node15, &node5, NULL, -1);
  initNode(&node5, NULL, NULL, 0);
  initNode(&node10, NULL, NULL, 0);

  Node *root = &node15;
  int heightChanged = addAvl(&root, &node10);
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
void test_addAvl_given_node5_node15_add_node10_expect_rotateRL(void){
  initNode(&node5, NULL, &node15, 1);
  initNode(&node15, NULL, NULL, 0);
  initNode(&node10, NULL, NULL, 0);

  Node *root = &node5;
  int heightChanged = addAvl(&root, &node10);

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
void test_addAvl_parent_noHeightChanged_add_10(void){
  initNode(&node5, &node1, &node15, 1);
  initNode(&node15, NULL, &node20, 1);
  initNode(&node1, NULL, NULL, 0);
  initNode(&node20, NULL, NULL, 0);

  initNode(&node10, NULL, NULL, 0);

  Node *root = &node5;
  int heightChanged = addAvl(&root, &node10);
  TEST_ASSERT_EQUAL(NO_CHANGED, heightChanged);
  TEST_ASSERT_EQUAL_NODE(&node5, &node1, &node15, 1);
  TEST_ASSERT_EQUAL_NODE(&node15, &node10, &node20, 0);

  //TODO change bf after rotate
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
void test_addAvl_given_above_expect_rotateRL(void){
  initNode(&node5, &node1, &node10, 1);
  initNode(&node10, NULL, &node20, 1);
  initNode(&node1, NULL, NULL, 0);
  initNode(&node20, NULL, NULL, 0);
  initNode(&node15, NULL, NULL, 0);

  Node *root = &node5;
  int heightChanged = addAvl(&root, &node15);
  TEST_ASSERT_EQUAL(NO_CHANGED, heightChanged);
  TEST_ASSERT_EQUAL_NODE(&node5, &node1, &node15, 1);
  TEST_ASSERT_EQUAL_NODE(&node15, &node10 , &node20, 0);

  //TODO change bf after rotate
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
void test_addAvl_given_above_expect_rotateRL__with_2_parents(void){
  initNode(&node3, &node2, &node5, 1);
  initNode(&node2, &node1, NULL, -1);
  initNode(&node5, &node4, &node10, 1);
  initNode(&node10, NULL, &node20, 1);
  initNode(&node1, NULL, NULL, 0);
  initNode(&node20, NULL, NULL, 0);
  initNode(&node4, NULL, NULL, 0);

  initNode(&node15, NULL, NULL, 0);

  Node *root = &node3;
  int heightChanged = addAvl(&root, &node15);
  TEST_ASSERT_EQUAL(NO_CHANGED, heightChanged);
  TEST_ASSERT_EQUAL_NODE(&node3, &node2, &node5, 1);
  TEST_ASSERT_EQUAL_NODE(&node5, &node4, &node15, 1);
  TEST_ASSERT_EQUAL_NODE(&node15, &node10 , &node20, 0);

  TEST_ASSERT_EQUAL_NODE(&node1, NULL , NULL, 0);
  TEST_ASSERT_EQUAL_NODE(&node10, NULL , NULL, 0);
  TEST_ASSERT_EQUAL_NODE(&node20, NULL , NULL, 0);
}

