/**
*
* hash_node.h : This is the node struct header file for HashNode, used in hash_table.cpp
*
* 01/16/25 - Created by H. Hui
*/

#include "dll_node.h"
#include "hash_node.h"

// Define a structure for a node in the hash table
HashNode::HashNode(int myKeyValue, DllNode* myDllNode) : key(myKeyValue), fifoNode(myDllNode), prev(nullptr), next(nullptr) {}

DllNode* HashNode::getFifoNode() {
	return fifoNode;
}