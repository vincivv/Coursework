/**************************************************************
* Class::  CSC-415-03 Fall 2025
* Name::  Almat Dias
* Student ID::  923022045
* GitHub-Name::  vincivv
* Project:: Assignment 2 – Stucture in Memory and Buffering
*
* File::  Almat_Dias_HW2_main.c
*
* Description::  Populate the personal info. Copy the message from the argument,
*                limit it to 100 characters with a null terminator at the end.
*                Read lines from getNext() into a 256 byte buffer, commit the buffer
*                when full. At the end, if there is any data in the buffer, pad it with
*                nulls and commit it. Finally, call checkIt() and return its value.
*
**************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assignment2.h"


int main(int argc, char *argv[]){
    
    //instantiate personalInfo
    struct personalInfo *infoPtr = malloc(sizeof(struct personalInfo));  

    //Populate the structure
    infoPtr->firstName = argv[1];
    infoPtr->lastName = argv[2];
    infoPtr->studentID = 923022045;
    infoPtr->level = SENIOR;
    infoPtr->languages = KNOWLEDGE_OF_C | KNOWLEDGE_OF_CPLUSPLUS | KNOWLEDGE_OF_JAVA | 
                         KNOWLEDGE_OF_PYTHON | KNOWLEDGE_OF_JAVASCRIPT | 
                         KNOWLEDGE_OF_MIPS_ASSEMBLER | 
                         KNOWLEDGE_OF_SQL | KNOWLEDGE_OF_HTML;
    
    // Copy message from argv[3] to infoPtr->message
    size_t i = 0;
    while (i < 99 && argv[3][i] != '\0') {
        infoPtr->message[i] = argv[3][i];
        i++;
    }
    infoPtr->message[i] = '\0';

    //Write the personal info
    writePersonalInfo(infoPtr);

    //Allocate buffer
    char *buffer = malloc(BLOCK_SIZE);
    size_t offset = 0;
    
    //Call getNext and put input in the buffer
    while(1){
        const char * line = getNext();
        if (!line) break;

        size_t remaining = strlen(line);
        const char *src = line;
        
        while (remaining > 0) { 
            size_t space = BLOCK_SIZE - offset;
            if (space == 0) {
                commitBlock(buffer);
                offset = 0;
                space = BLOCK_SIZE;
            }
            size_t chunk = (remaining < space) ? remaining : space;
            memcpy(buffer + offset, src, chunk);
            offset += chunk;
            src += chunk;
            remaining -= chunk;

            if (offset == BLOCK_SIZE) {
                commitBlock(buffer);
                offset = 0;
            }
        }
    }

    if (offset > 0) {
        memset(buffer + offset, 0, BLOCK_SIZE - offset);
        commitBlock(buffer);
    }

    //Check results
    int result = checkIt();

    //Free allocated memory
    free(buffer);
    free(infoPtr);

    return result;
}