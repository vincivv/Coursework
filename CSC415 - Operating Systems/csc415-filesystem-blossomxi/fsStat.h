/**************************************************************
* Class::  CSC-415-03 Fall 2025
* Name:: Joshua Muhammad
* Student IDs:: 924074746
* GitHub-Name:: JoshuaM928
* Group-Name:: Four Blobs
* Project:: Basic File System
*
* File:: fsStat.h
*
* Description:: This file declares the fs_stat function which is used by programs like 'ls' to give
* more detailed information.
*
**************************************************************/

#ifndef FSSTAT_H
#define FSSTAT_H

#define FSSTAT_SUCCESS 0
#define FSSTAT_FAILURE -1

#include "mfs.h"

/**
 * @brief
 *
 * @param path path to target file or directory.
 * @param buf struct fs_stat we're going to populate.
 * @return int
 * @retval 0 success
 * @retval -1 failure
 */
int fs_stat(const char *path, struct fs_stat *buf);

#endif // FSSTAT_H