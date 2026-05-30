/**************************************************************
* Class::  CSC-415-03 Fall 2025
* Name:: Dias Almat
* Student ID:: 923022045
* GitHub-Name:: vincivv
* Project:: Assignment 1 – Command Line Arguments
*
* File:: Almat_Dias_HW1_main.c
*
* Description::
*
**************************************************************/

#include <stdio.h>

int main(int argc, char *argv[]) {
    printf("\n\nThe argument count is: %d\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("Argument %d is: %s\n", i, argv[i]);
    }
    return 0;
}
