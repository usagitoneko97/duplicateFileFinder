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

#define initNodeStr(node, left, right, bf)  initNode((Node*)node, (Node*)left, (Node*)right, bf)

CEXCEPTION_T ex;
void setUp(void)
{
  initStringNodeData();
}

void tearDown(void)
{
}

void test_avlString_cmp_ali_abu_expect_ali_greater_than_abu_return_1(void){

  char *ali = "Ali";
  Node *nodeAbuNode = (Node*)&nodeAbu;
  TEST_ASSERT_EQUAL(1, compareStrVoidPtr((void*)ali, nodeAbuNode));
}

void test_avlString_cmp_abuBaba_abu_expect_abuBaba_greater_than_abu_return_1(void)
{

  char *ali = "AbuBaba";
  Node *nodeAbuNode = (Node *)&nodeAbu;
  TEST_ASSERT_EQUAL(1, compareStrVoidPtr((void *)ali, nodeAbuNode));
}

void test_avlString_cmp_abu_abu_expect_same_return_0(void)
{

  char *ali = "Abu";
  Node *nodeAbuNode = (Node *)&nodeAbu;
  TEST_ASSERT_EQUAL(0, compareStrVoidPtr((void *)ali, nodeAbuNode));
}

void test_avlString_cmp_Aakaikoen_abu_expect_Aakaikoen_smaller_than_abu_return_m1(void)
{
  char *ali = "Aakaikoen";
  Node *nodeAbuNode = (Node *)&nodeAbu;
  TEST_ASSERT_EQUAL(-1, compareStrVoidPtr((void *)ali, nodeAbuNode));
}
/**
       add Ali, Abu, Baba
*  NULL       --->                Ali
*                                /  \
*                              Abu  Baba
*/
void test_add_ali_abu_baba_to_NULL_expect_balancedTree(void){
  StrNode *root = NULL;
  Try{
    avlAddString(&root, &nodeAli);
    avlAddString(&root, &nodeAbu);
    avlAddString(&root, &nodeBaba);
  }Catch(ex){
    dumpException(ex);
  }

  TEST_ASSERT_EQUAL_NODE(&nodeAli, &nodeAbu, &nodeBaba, 0);
}
/**
       add Ali, Baba, hgx
*  NULL       --->             Ali                 Baba
*                               \    rotateLeft   /   \
*                              Baba    --->      Ali   Hgx
*                                 \
*                                  Hgx
*/
void test_add_ali_baba_hgx_to_NULL_expect_balancedTree(void){
  StrNode *root = NULL;
  Try{
    avlAddString(&root, &nodeAli);
    avlAddString(&root, &nodeBaba);
    avlAddString(&root, &nodeHgx);
  }Catch(ex){
    dumpException(ex);
  }

  TEST_ASSERT_EQUAL_NODE(&nodeBaba, &nodeAli, &nodeHgx, 0);
}


/** 
 *     Baba   delete Ali   Baba
 *    /  \      ---->        \
 *  Ali  Hgx                 Hgx
 */
void test_delete_ali_baba_hgx_delete_baba_expect_balanceTree(void){
  StrNode *root = NULL;
  Try
  {
    avlAddString(&root, &nodeAli);
    avlAddString(&root, &nodeBaba);
    avlAddString(&root, &nodeHgx);
    avlRemoveStringWithValue(&root, "Ali");
    
  }
  Catch(ex)
  {
    dumpException(ex);
  }
  TEST_ASSERT_EQUAL_NODE(&nodeBaba, NULL, &nodeHgx, 1);
}