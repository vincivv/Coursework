/**************************************************************
* Class::  CSC-415-03 Fall 2025
* Name:: John Rilles
* Student IDs:: 922591914
* GitHub-Name:: blossomxi
* Group-Name:: Four Blobs
* Project:: Basic File System
*
* File:: john.c
*
* Description:: This holds some of the functions for the file system
*  such as mkdir, open, and rmdir.
*
**************************************************************/

#include "freemap.h"
#include "fsLow.h"
#include "mfs.h"
#include "parsePath.h"
#include "rootDir.h"
#include "volumeControlBlock.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
extern VCB vcb;

int allocateBlock() {
  for (int i = 0; i < vcb->numBlock; i++) {
    if (isBlock(i)) {
      vcb->freeSpace--;
      return i;
    }
  }
  return -1; // no allocation block found
}
// Creating the mkdir & rmdir

int fs_mkdir(const char *pathname, mode_t mode) {

  // saves the path to a character variable
  char *path = malloc(strlen(pathname) + 1);
  strcpy(path, pathname);
  // allocates the space for the ppInfo
  ppInfo *info = malloc(sizeof(ppInfo));
  parsePath(path, info);
  // TODO Need to find a way to load the data of the ppInfo

  // Error checks if path is valid
  if (info->isValid == 0) {
    return -1;
  }
  // TODO: Checks if the last index of the path exists
  if (info->index != TARGET_NOT_FOUND_IN_PARENT) {
    return -1;
  }

  // TODO: if directory is existing then return a fail

  // initializes the startblock
  int startBlock = allocateBlock();
  if (startBlock == -1) {
    return -1;
  }

  

  // TODO: Writes creates the directory

  DE *newDirectory = createDir(50, info->parent, 
                  vcb->sizeOfBlock, startBlock);
  //Write the dir into the fs
  // initialize the new directory
  LBAWrite(startBlock, newDirectory, vcb->sizeOfBlock);

 free(path);
 free(info);
  return 0;
}

int fs_rmdir(const char *pathname) {
  // TODO
  char *path = malloc(strlen(pathname) + 1);
  strcpy(path, pathname);

  ppInfo ppi;
  parsePath(path, &ppi);
  // If the path and the index is invalid return
  // an invalid  and free the parsepath
  if (ppi.isPathValid == 0 || ppi.index < 0) {
    return -2;
  }
  DE *parent = ppi.parent;
  // if the dir is found we need to delete the 
  DE *dirDelete = &parent[ppi.index];

  if (dirDelete->size > 0) {
    free(path);
    return -1;
  }

  dirDelete->inUse = 0;
  free(path);
  return 0;
}

fdDir *fs_opendir(const char *pathname) {

  char *path = malloc(strlen(pathname) + 1);
  ppInfo ppi;
  strcpy(path, path);
  parsePath(path, &ppi);
  
  fdDir *fdDir = malloc(sizeof(fdDir));
  if (!fdDir){
    return NULL;
  }
  fdDir->d_reclen = sizeof(fdDir);
  fdDir->dirEntryPosition = 0;
  fdDir->directory = ppi.index;

  return fdDir;
}
