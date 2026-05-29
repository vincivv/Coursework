/**************************************************************
* Class::  CSC-415-03 Fall 2025
* Name:: John Rilles, Joshua Muhammad
* Student IDs:: 922591914, 924074746
* GitHub-Name:: blossomxi, JoshuaM928
* Group-Name:: Four Blobs
* Project:: Basic File System
*
* File:: parsePath.h
*
* Description:: This file contains the declaration for the parse path function, which allows us to
* determine wether or not the given path is valid or not. This allows us to abstract away the idea
* of paths to the rest of the file system as this will allow us to simply get the info of the target
* of the specified path.
*
**************************************************************/

#ifndef PARSEPATH_H
#define PARSEPATH_H

#define PARSE_PATH_SUCCESS 0
#define NO_OR_EMPTY_PATH -1
#define TARGET_NOT_FOUND_IN_PARENT -1
#define PATH_IS_ROOT -2
#define PATH_NOT_EXIST -2
#define PATH_INCLUDES_FILE_BEFORE_TARGET -3

#include "rootDir.h"

typedef struct ppInfo {
  DE *parent; // Pointer to the parent directory containing the blob the target blob.
  char *lastElementName; // Name of the target blob.
  int index;             // Directory entry index in the parent directory.
} ppInfo;

/**
 * @brief Helper function used to get the parent directory of the target blob.
 *
 * @param path Path to the target.
 * @param ppi Pointer to the ppInfo structure.
 *
 * @return int Status code.
 * @retval 0   Success.
 * @retval -1  No path or empty path provided.
 * @retval -2  Target doesn't exist in parent DE.
 * @retval -3  Path includes a file before target.
 */
int parsePath(const char *path, ppInfo *ppi);

/**
 * @brief Helper function used in parsePath to determine a token's index in the
 * parent directory. if the return value is -1 then the token could not be found
 * in the parent directory.
 *
 * @param parent pointer to parent directory entry.
 * @param token name of the target directory entry you're searching for in the
 * parent directory
 * @return int status code.
 * @retval -1 token couldn't be found in the parent directory.
 * @retval >= 0 index of the target in the parent directory.
 */
int findInDirectory(DE *parent, char *token);

/**
 * @brief Helper function used in parsePath to determine if the current
 * directory entry is a directory.
 *
 * @param parent pointer to parent directory.
 * @return int status code
 * @retval 1 the directory entry is a directory.
 * @retval 0 the directory entry is a file.
 */
int isDEaDir(DE *parent);

/**
 * @brief Loads the directory specified by parent from disk into memory
 *
 * @param parent pointer to parent directory.
 * @return DE * newParent
 */
DE *loadDirectory(DE *parent);

#endif // PARSEPATH_H