/**
* @author - Hugh Hui
* @file TreeNode.cpp
* @brief Constructor for the TreeNode class.
*
* 1/14/2025 - H. Hui created file and added comments.
* 1/15/2025 - modified by H. Hui; modified remove methods; added comments
* 2/1/2025 - H. Hui added doxygen formatted comments
*
* This constructor initializes a TreeNode object with the provided key, number of nodes,
* height, and pointers to the left and right children.
*
* @param k The key of the node.
* @param numNodes The total number of nodes in the subtree rooted at this node.
* @param h The height of the node in the tree.
* @param l Pointer to the left child node.
* @param r Pointer to the right child node.
*
*/
#include "tree_node.h"

TreeNode::TreeNode(int k, int numNodes, int h, TreeNode* l, TreeNode* r, DllNode* f)
    : key(k), numberOfNodes(numNodes), height(h), left(l), right(r), fifoNode(f) {}

TreeNode::TreeNode(int k, DllNode* f) {
    key = k;
    numberOfNodes = 0;
    height = 0;
    left = nullptr;
    right = nullptr;
    fifoNode = f;
}
