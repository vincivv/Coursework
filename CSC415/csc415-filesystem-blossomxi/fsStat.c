/**************************************************************
* Class::  CSC-415-03 Fall 2025
* Name:: Joshua Muhammad
* Student IDs:: 924074746
* GitHub-Name:: JoshuaM928
* Group-Name:: Four Blobs
* Project:: Basic File System
*
* File:: fsStat.c
*
* Description:: The fs_stat function defined in this file fills the caller's buffer with additional
* information about the target of the given path.
*
**************************************************************/

#include "fsStat.h"
#include "parsePath.h"
#include <stdlib.h>

extern blksize_t st_blksize;

/**
 * @brief fs_stat is responsible for giving the caller more information about
 * the target directory entry at the desired path, it then writes the data it
 * finds about the directory entry to the callers buffer. Various return values
 * not equal to zero indicate some sort of failure.
 *
 * @param path
 * @param buf
 * @return int
 */
int fs_stat(const char *path, struct fs_stat *buf) {
  if (!path || !buf) {
    printf("the path and/or buffer passed to fs_stat is NULL\n");
    return FSSTAT_FAILURE;
  }

  ppInfo *ppInfo_p = malloc(sizeof(ppInfo));
  if (!ppInfo_p) {
    printf("failed to malloc space for ppInfo struct in fs_stat\n");
    return FSSTAT_FAILURE;
  }

  if (parsePath(path, ppInfo_p) != PARSE_PATH_SUCCESS) {
    printf("parse path failed in fs_stat\n");
    return FSSTAT_FAILURE;
  }

  DE *currDir = malloc(sizeof(DE));
  if (!currDir) {
    printf("failed to allocate space for directory entry in fs_stat\n");
    return FSSTAT_FAILURE;
  }
  currDir = ppInfo_p->parent;
  free(ppInfo_p);
  ppInfo_p = NULL;

  buf->st_size = currDir->size;
  buf->st_blksize = st_blksize;
  buf->st_blocks = (currDir->size + (st_blksize - 1)) / st_blksize;
  buf->st_accesstime = currDir->accessed;
  buf->st_modtime = currDir->lastModified;
  buf->st_createtime = currDir->created;

  free(currDir);
  currDir = NULL;

  return FSSTAT_SUCCESS;
}