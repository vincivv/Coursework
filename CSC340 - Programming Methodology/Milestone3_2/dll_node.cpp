/**
*
* @author - Hugh Hui
* @file dll_node.cpp -  This file contains the constructor.

12/30/2024 - initial creation

*/

#include <iostream>
#include <fstream>
#include <string>

#include "dll_node.h"

extern void logToFileAndConsole(std::string msg);

// Constructor to initialize a node with a specific value
DllNode::DllNode(int value) : key(value), prev(nullptr), next(nullptr) {}

DllNode::DllNode(int myValue, std::string myFullName, std::string myAddress, std::string myCity, std::string myState, std::string myZip) : key(myValue), fullName(myFullName), address(myAddress), city(myCity), state(myState), zip(myZip), prev(nullptr), next(nullptr) {}

void DllNode::printNode() {
    bool verbose = false;
    if (verbose) {
        logToFileAndConsole("FIFO info from cacheManager.  Node key: " + std::to_string(key) + "; name: " + fullName + ";address: " + address + "; city: " + city + "; state: " + state + "; zip: " + zip);
    }
    else {
        logToFileAndConsole("FIFO info from cacheManager:  Node key: " + std::to_string(key));
    }
}
