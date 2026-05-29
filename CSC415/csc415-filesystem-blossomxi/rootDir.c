/**************************************************************
* Class::  CSC-415-03 Fall 2025
* Name:: Joshua Muhammad, John Rilles, Dias Almat
* Student IDs:: 924074746, 922591914, 923022045
* GitHub-Name:: JoshuaM928, blossomxi, vincivv
* Group-Name:: Four Blobs
* Project:: Basic File System
*
* File:: rootDir.c
*
* Description:: Create and initialize a root directory for the file system. 
*
**************************************************************/

#include "fsLow.h"
#include "rootDir.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

DE *currentDir = NULL;
DE * rootDir = NULL;

DE * createDir(int minDirEntries, DE * parent, int blockSize, int startBlock){
    //parts a - d
    int bytesNeeded = minDirEntries * sizeof(DE); //How many byte needed for the directory
    int blocksNeeded = (bytesNeeded + (blockSize - 1)) / blockSize; //How many block needed in the disk
    int bytesToMalloc = blocksNeeded * blockSize; //How many bytes needed to malloc
    int actualEntries = bytesToMalloc / sizeof(DE); //Check if we can fit an extra entry


    DE *new = malloc(bytesToMalloc);
    if(new == NULL) return NULL;


    //part e, initialize each entry to a known free state
    for(int i = 2; i < actualEntries; i++){
        new[i].name[0] = '\0';
        new[i].inUse = 0;
        /*
        new[i].cation = -1;
        new[i].isDirectory = 0;
        new[i].size = 0;
        new[i].accessed = 0;
        new[i].lastModified = 0;
        new[i].created = 0;
        */
    }

    //part f
    //
    int loc = startBlock;


    //part g, set the . entry
    time_t curDT = time(NULL);
    strcpy(new[0].name, ".");
    new[0].size = actualEntries * sizeof(DE);
    new[0].created = curDT;
    new[0].lastModified = curDT;
    new[0].accessed = curDT;
    new[0].inUse = 1;
    new[0].isDirectory = 1;
    if(loc == 0){
        free(new);
        new = NULL;
        return NULL;
    }
    new[0].location = loc;


    //part h, set the .. entry
    if(parent == NULL){
        parent = new;
    }
    strcpy(new[1].name, "..");
    new[1].size = parent->size;
    new[1].location = parent->location;
    new[1].created= parent -> created;
    new[1].accessed = parent -> accessed;
    new[1].lastModified = parent -> lastModified;
    new[1].inUse = parent -> inUse;
    new[1].isDirectory = parent -> isDirectory;


    //part i
    //Not sure if I did this right, might need to call some outide func
    int writeToDisk = LBAwrite(new, blocksNeeded, loc);

    // Initialize root directory global pointer
    rootDir = malloc(bytesToMalloc);
    if (!rootDir) {
        printf("failed to malloc space for global root directory pointer.\n");
        free(new);
        new = NULL;
        return NULL;
    }

    if(writeToDisk != blocksNeeded){
        printf("failed to write the correct number of blocks needed for root directory.\n");
        free(new);
        new = NULL;
        free(rootDir);
        rootDir = NULL;
        return NULL;
    }

    //part j
    //Not sure what professor is asking for this part
    currentDir = malloc(bytesToMalloc);
    if (!currentDir) {
        printf("failed to malloc space for global current directory pointer.\n");
        free(new);
        new = NULL;
        free(rootDir);
        rootDir = NULL;
        return NULL;
    }

    return(new);
}