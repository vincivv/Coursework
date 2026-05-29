/**************************************************************
* Class::  CSC-415-03 Fall 2025
* Name:: Dias Almat
* Student IDs:: 923022045
* GitHub-Name:: vincivv
* Group-Name:: Four Blobs
* Project:: Basic File System
*
* File:: isFile_isDir.c
*
* Description:: 
*
* 
*
**************************************************************/

#include "mfs.h"
#include "volumeControlBlock.h"
#include "rootDir.h"
#include "parsePath.h"

int fs_isFile(char *fileName){
    // Check if the path is valid
    if(fileName == NULL) {
        return -1; // Invalid path
    }

    if (strcmp(fileName, "/") == 0) {
        return 0; // Root is a directory, not a file
    }

    char pathCopy[1024];
    size_t len = strlen(fileName);
    if (len >= sizeof(pathCopy)) {
        return -1; // Path too long
    }
    strcpy(pathCopy, fileName);

    ppInfo info; // local ppInfo, no global pointer

    int status = parsePath(pathCopy, &info);
    if (status != PARSE_PATH_SUCCESS) {
        // PATH_NOT_EXIST, PATH_INCLUDES_FILE_BEFORE_TARGET, etc.
        return -2; // File does not exist / invalid path
    }

    if (info.index < 0) {
        return -2; // File does not exist
    }

    // Get the directory entry for the target
    DE *entry = &info.parent[info.index];

    // Check if the entry is a file
    if(entry ->isDirectory == 0 && entry -> inUse == 1){
        return 1; 
    } else {
        return 0; 
    }
} 

int fs_isDir(char *pathName){
    // Check if the path is valid
    if(pathName == NULL) {
        return -1;
    }

    if (strcmp(pathName, "/") == 0) {
        return 1; // Root is a directory
    }

    char pathCopy[1024];
    size_t len = strlen(pathName);
    if (len >= sizeof(pathCopy)) {
        return -1; // Path too long
    }
    strcpy(pathCopy, pathName);

    ppInfo info;

    int status = parsePath(pathCopy, &info);
    if (status != PARSE_PATH_SUCCESS) {
        return -2; // Path does not exist or is invalid
    }

    if (info.index < 0) {
        return -2; // Target not found in parent
    }

    DE* entry = &info.parent[info.index];

    // Check if the entry is a directory
    if(entry ->isDirectory == 1 && entry -> inUse == 1){
        return 1;
    } else {
        return 0;
    }
}