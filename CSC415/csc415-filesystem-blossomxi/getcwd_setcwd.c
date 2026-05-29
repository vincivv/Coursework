/**************************************************************
* Class::  CSC-415-03 Fall 2025
* Name:: Dias Almat
* Student IDs:: 923022045
* GitHub-Name:: vincivv
* Group-Name:: Four Blobs
* Project:: Basic File System
*
* File:: getcwd_setcwd.c
*
* Description::
*
* 
*
**************************************************************/

#include "mfs.h"
#include "volumeControlBlock.h"
#include "parsePath.h"
#include "rootDir.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char currentDirPath[1024] = "/"; // the Path of the current working directory

char* fs_getcwd(char* pathname, size_t size) {
    if (pathname == NULL || size == 0) {
        return NULL; // Invalid buffer
    }

    size_t cwdLength = strlen(currentDirPath);
    if (cwdLength + 1 > size) {
        return NULL; // Buffer too small
    }

    strncpy(pathname, currentDirPath, size);
    return pathname;
}

int fs_setcwd(const char* pathname) {
    if (pathname == NULL) {
        return -1; // Invalid path or path too long
    }

    char pathCopy[1024];
    size_t len = strlen(pathname);
    if (len >= sizeof(pathCopy)) {
        return -1; // Path too long for the buffer
    }
    strcpy(pathCopy, pathname);

    ppInfo info;

    int status = parsePath(pathCopy, &info);
    if (status != PARSE_PATH_SUCCESS) {
        return -2; // Path does not exist or is invalid
    }

    // if the target is root
    if (info.lastElementName == NULL &&
        info.index == PATH_IS_ROOT &&
        info.parent == rootDir) {
        currentDir = rootDir;
        strncpy(currentDirPath, "/", sizeof(currentDirPath) - 1);
        currentDirPath[sizeof(currentDirPath) - 1] = '\0';
        return 0;
    }

    if (info.index < 0) {
        return -2; // Target not found in parent
    }

    // Get the directory entry for the target
    DE* entry = &info.parent[info.index];

    if (!isDEaDir(entry)) {
        return -2; // Path does not exist or is not a directory
    }

    // Load the new directory into memory and make it the current directory
    DE* newDir = loadDirectory(entry);
    if (newDir == NULL) {
        return -2; // Failed to load directory from disk
    }

    currentDir = newDir; // change currentDir to the new directory
    strncpy(currentDirPath, pathname, sizeof(currentDirPath) - 1);
    currentDirPath[sizeof(currentDirPath) - 1] = '\0';

    return 0; 
}

