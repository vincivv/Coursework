/**
*
* @author - Hugh Hui
* @file node.cpp -  This file contains the constructor.

12/30/2024 - initial creation

*/

#include "dll_node.h"

// Constructor to initialize a node with a specific value
DllNode::DllNode(int value) : key(value), prev(nullptr), next(nullptr) {}
