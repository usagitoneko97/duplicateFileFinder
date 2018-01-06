#include "NodeHelper.h"

extern Node node2, node3, node4;
extern Node node1, node5, node10, node15, node20, node25, node30, node35, node40, node23;
extern Node node45, node50, node55;


void initNodeData(void){
  node1.data = 1;
  node2.data = 2;
  node3.data = 3;
  node4.data = 4;
  node5.data = 5;
  node7.data = 7;
  node10.data = 10;
  node15.data = 15;
  node20.data = 20;
  node25.data = 25;
  node30.data = 30;
  node35.data = 35;
  node40.data = 40;
  node45.data = 45;
  node50.data = 50;
  node55.data = 55;
  node60.data = 60;
  node70.data = 70;
  node23.data = 23;
}

void initNode(Node *node, Node *left, Node *right, int bf){
  node->left = left;
  node->right = right;
  node->bf =bf;
}
