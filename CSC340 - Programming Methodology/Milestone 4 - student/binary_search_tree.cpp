/**
* @author - Dias Almat
* @file binary_search_tree.cpp -  file with the method implementations for the BinarySearchTree class.
* 04/12/2025 - D.Almat created file and added comments.
*/

#include "binary_search_tree.h"
#include <iostream>
#include <queue>
#include <functional>

BinarySearchTree::BinarySearchTree() : root(nullptr) {}     //Constructor

BinarySearchTree::~BinarySearchTree() {                     //Destructor
    deleteTree(root);
}

// Add a node to the tree
void BinarySearchTree::addToTree(int key) {
    TreeNode* newNode = new TreeNode(key);
    if (!root) {
        root = newNode;
        return;
    }

    TreeNode* current = root;
    while (true) {
        current->numberOfNodes++;
        if (key < current->key) {
            if (!current->left) {
                current->left = newNode;
                break;
            }
            current = current->left;
        } else {
            if (!current->right) {
                current->right = newNode;
                break;
            }
            current = current->right;
        }
    }

    root->height = getHeight(root);
}

// Remove a node from the tree
bool BinarySearchTree::removeNode(int key) {
    TreeNode* parent = nullptr;
    TreeNode* current = root;

    while (current && current->key != key) {
        parent = current;
        current = (key < current->key) ? current->left : current->right;
    }

    if (!current) return false;

    TreeNode* replacement;
    if (!current->left) {
        replacement = current->right;
    } else if (!current->right) {
        replacement = current->left;
    } else {
        TreeNode* succParent = current;
        TreeNode* successor = current->right;
        while (successor->left) {
            succParent = successor;
            successor = successor->left;
        }
        current->key = successor->key;
        return removeNode(successor->key);
    }

    if (!parent) {
        delete root;
        root = replacement;
    } else if (parent->left == current) {
        delete current;
        parent->left = replacement;
    } else {
        delete current;
        parent->right = replacement;
    }

    root->height = getHeight(root);
    return true;
}

//height of the tree
int BinarySearchTree::getHeightOfTree() const {
    return getHeight(root);
}

//number of nodes in the tree
int BinarySearchTree::getNumberOfTreeNodes() const {
    std::function<int(TreeNode*)> count = [&](TreeNode* node) {
        if (!node) return 0;
        return 1 + count(node->left) + count(node->right);
    };
    return count(root);
}

// Check if the tree contains a key
bool BinarySearchTree::contains(int key) const {
    TreeNode* current = root;
    while (current) {
        if (key == current->key) return true;
        current = (key < current->key) ? current->left : current->right;
    }
    return false;
}

//Return the root of the tree
TreeNode* BinarySearchTree::getRoot() const {
    return root;
}

// Check if the tree is empty
bool BinarySearchTree::isEmpty() const {
    return root == nullptr;
}

//Clear the tree
void BinarySearchTree::clear() {
    deleteTree(root);
    root = nullptr;
}

// Print the node from the tree
void BinarySearchTree::printNodeFromTree(TreeNode* node) const {
    if (node) {
        std::cout << "Node key: " << node->key << std::endl;
    }
}

// Print the tree in in order
void BinarySearchTree::printInOrder() const {
    std::cout << "Performing In-order traversal" << std::endl;
    printInOrderHelper(root);
}

// Print the tree in pre order
void BinarySearchTree::printPreOrder() const {
    std::cout << "Performing Pre-order traversal" << std::endl;
    printPreOrderHelper(root);
}

// Print the tree in post order
void BinarySearchTree::printPostOrder() const {
    std::cout << "Performing Post-order traversal" << std::endl;
    printPostOrderHelper(root);
}

// Print the tree in depth first
void BinarySearchTree::printDepthFirst() const {
    std::cout << "Performing Depth First via PreOrder traversal" << std::endl;
    std::cout << "Performing Pre-order traversal" << std::endl;
    printPreOrderHelper(root);
}

// Print the tree in breadth first
void BinarySearchTree::printBreadthFirst() const {
    std::cout << "Performing Breadth First traversal" << std::endl;
    if (!root) return;

    std::queue<TreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        TreeNode* current = q.front();
        q.pop();

        std::cout << "Node key: " << current->key << std::endl;

        if (current->left) q.push(current->left);
        if (current->right) q.push(current->right);
    }
}

//delete the tree
void BinarySearchTree::deleteTree(TreeNode* node) {
    if (!node) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

// Get the height of the tree
int BinarySearchTree::getHeight(TreeNode* node) const {
    if (!node) return 0;
    int leftHeight = getHeight(node->left);
    int rightHeight = getHeight(node->right);
    return 1 + std::max(leftHeight, rightHeight);
}

//Helper function for in order traversal
void BinarySearchTree::printInOrderHelper(TreeNode* node) const {
    if (!node) return;
    printInOrderHelper(node->left);
    std::cout << "Node key: " << node->key << std::endl;
    printInOrderHelper(node->right);
}

//Helper function for pre order traversal
void BinarySearchTree::printPreOrderHelper(TreeNode* node) const {
    if (!node) return;
    std::cout << "Node key: " << node->key << std::endl;
    printPreOrderHelper(node->left);
    printPreOrderHelper(node->right);
}


//Helper function for post order traversal
void BinarySearchTree::printPostOrderHelper(TreeNode* node) const {
    if (!node) return;
    printPostOrderHelper(node->left);
    printPostOrderHelper(node->right);
    std::cout << "Node key: " << node->key << std::endl;
}