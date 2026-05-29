/**************************************************************
 * Class::  CSC-415-03 Fall 2025
 * Name:: Joshua Muhammad
 * Student IDs:: 924074746
 * GitHub-Name:: JoshuaM928
 * Group-Name:: Four Blobs
 * Project:: Basic File System
 *
 * File:: test_parse_path.c
 *
 * Description:: Function to allow basic testing of ../parsePath.c
 *
 **************************************************************/

#include "test_parse_path.h"

#include <stdio.h>

int main() {
  char path[] = "/";
  ppInfo ppi = {0};

  return testParse((const char *)&path, &ppi);
}

int testParse(const char *path, ppInfo *ppi) {
  if (parsePath(path, ppi) == PARSE_PATH_SUCCESS) {
    printf("parsePath succeeded\n");
  } else {
    printf("parsePath test failed\n");
  }

  return 0;
}