/**************************************************************
* Class::  CSC-415-03 Fall 2025
* Name:: John Rilles, Joshua Muhammad
* Student IDs:: 922591914, 924074746
* GitHub-Name:: blossomxi, JoshuaM928
* Group-Name:: Four Blobs
* Project:: Basic File System
*
* File:: parsePath.c
*
* Description:: The implementation of parse path below allows us to pass in any path and give the
* caller the relevant information about the target path so that they can continue working.
*
**************************************************************/

#include "parsePath.h"

#include <stdlib.h>
#include <string.h>

extern DE *currentDir;
extern DE *rootDir;

int parsePath(const char *path, ppInfo *ppi) {
  // TODO: Add comments that explain the logic of the code.
  DE *startParent = {0};
  DE *parent = {0};
  char *savePtr = NULL;
  char *token1 = NULL;
  char *token2 = NULL;

  if (!path) {
    return NO_OR_EMPTY_PATH;
  }

  if (path[0] == '/') {
    startParent = rootDir;
  } else {
    startParent = currentDir;
  }
  parent = startParent;

  token1 = strtok_r(path, '/', &savePtr);

  if (token1 == NULL) {
    if (startParent == rootDir) {
      ppi->parent = parent;
      ppi->lastElementName = NULL;
      ppi->index = PATH_IS_ROOT;
      return PARSE_PATH_SUCCESS;
    } else {
      return NO_OR_EMPTY_PATH;
    }
  }

  while (1) {
    int index = findInDirectory(parent, token1);
    token2 = strtok_r(NULL, '/', &savePtr);

    if (token2 == NULL) {
      ppi->parent = parent;
      ppi->lastElementName = token1;
      ppi->index = index;
      return PARSE_PATH_SUCCESS;
    } else {
      if (index == TARGET_NOT_FOUND_IN_PARENT) {
        return PATH_NOT_EXIST;
      }
      if (!isDEaDir(&(parent[index]))) {
        return PATH_INCLUDES_FILE_BEFORE_TARGET;
      }
      DE *tempParent = loadDirectory(&(parent[index]));
      if (parent != startParent) {
        free(parent);
        parent = NULL;
      }
      parent = tempParent;
      token1 = token2;
    }
  }
}