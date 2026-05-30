/**************************************************************
* Class::  CSC-415-03 Fall 2025
* Name::  Dias Almat
* Student ID::  923022045
* GitHub-Name::  vincivv
* Project:: Assignment 3 – Simple Shell with Pipes
*
* File:: Almat_Dias_HW3_main.c
*
* Description::  This file first reads the input from CLI, and saves everything to a fixed-size buffer.
*               Then it uses delimiter to split the input by pipes. Then it splits each
*               pipe argument into command and its arguments. If there are no pipes.
*               If there are no pipes, it creates a new process and executs. 
*               If there are pipes, it creates the fds for the pipes, creates a new process for each command,
*               sets up the pipes, and executes the commands. Then it closes the fds and waits for all children to finish.
*
**************************************************************/

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define BLOCK_SIZE 91
#define MAX_ARGS 46
#define MAX_STAGES 23

int main(int argc, char const *argv[]){
    
    char buffer[BLOCK_SIZE];

    //Set prompt
    char *prompt;
    if(argc > 1 && argv[1] != NULL){
            prompt = (char *)argv[1];
        }

    while(1){

        printf("%s ", prompt);
        //Read input from user
        char * line = fgets(buffer, BLOCK_SIZE, stdin);
    
        //Handle EOF or error
        if(line == NULL){
            if(feof(stdin) == 1){
                exit(0);
            }
            else if(ferror(stdin)){
                perror("Error reading input");
                exit(1);
            }
        }

        //Split the input by pipes
        char *pipeArgs[MAX_STAGES];
        char *pipeToken = strtok(buffer, "|");
        size_t pipeCount = 0;
        while(pipeToken != NULL && pipeCount < MAX_STAGES){
            pipeArgs[pipeCount++] = pipeToken;
            pipeToken = strtok(NULL, "|");
        }
        pipeArgs[pipeCount] = NULL;


        //Split each pipe argument into command and its arguments
        char *args[pipeCount][MAX_ARGS + 1]; 
        for(size_t i = 0; i < pipeCount; i++){
            size_t argCount = 0;
            char *argToken = strtok(pipeArgs[i], " \t\r\n");
            while(argToken != NULL && argCount < MAX_ARGS){
                args[i][argCount++] = argToken;
                argToken = strtok(NULL, " \t\r\n");
            }
            args[i][argCount] = NULL;
        }

        //Check if user entered an empty string or exited
        if(args[0][0] == NULL){
            printf("Empty string entered:  Nothing to do.\n");
            continue;    
        }
        if(strcmp(args[0][0], "exit") == 0){
            exit(0);
        }

        
        //No pipes, single command
        if(pipeCount == 1){
            //Create a new process and execute
            __pid_t pid = fork();
            if(pid < 0){
                perror("Fork failed");
                exit(1);
            }else if(pid == 0){
                //Child process
                execvp(args[0][0], args[0]);
                perror("Execution failed");
                exit(2);
            }else{
                //Parent process
                int status;
                waitpid(pid, &status, 0);
                if(WIFEXITED(status)){
                    printf("Child %d exited with status %d\n", pid, WEXITSTATUS(status));
                }
            }      
        }

        //Multiple commands with pipes
        else{
            int fds[MAX_STAGES - 1][2];
            for(size_t i = 0; i < pipeCount - 1; i++){
                if(pipe(fds[i]) < 0){
                    perror("Pipe creation failed");
                    exit(1);
                }
            }

            __pid_t pids [pipeCount];
            for(size_t i = 0; i < pipeCount; i++){
                //Create a new process for each command
                __pid_t pid = fork();
                if(pid < 0){
                    perror("Fork failed");
                    exit(1);
                }else if(pid == 0){
                    //Child process
                    if(i > 0){
                        dup2(fds[i - 1][0], STDIN_FILENO);
                    }
                    if(i < pipeCount - 1){
                        dup2(fds[i][1], STDOUT_FILENO);
                    }
                    //Close all fds in child
                    for(size_t j = 0; j < pipeCount - 1; j++){
                        close(fds[j][0]);
                        close(fds[j][1]);
                    }
                    execvp(args[i][0], args[i]);
                    perror("Execution failed");
                    exit(2);
                }else{
                    //Parent process
                    pids[i] = pid;
                }
            }
            //Close all fds in parent
            for(size_t i = 0; i < pipeCount - 1; i++){
                close(fds[i][0]);
                close(fds[i][1]);
            }
            //Wait for all children to finish
            for(size_t i = 0; i < pipeCount; i++){
                int status;
                waitpid(pids[i], &status, 0);
                if(WIFEXITED(status)){
                    printf("Child %d exited with status %d\n", pids[i], WEXITSTATUS(status));
                }
            }
        }
    }
}
