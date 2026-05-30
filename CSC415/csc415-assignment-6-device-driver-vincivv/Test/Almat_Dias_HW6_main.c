/**************************************************************
* Class::  CSC-415-03 Spring 2024
* Name::  Almat Dias
* Student ID:: 923022045
* GitHub-Name:: vincivv
* Project:: Assignment 6 – Coding A Device Driver
*
* File:: Almat_Dias_HW6_main.c
*
* Description::     This is a user-space application that interacts with the RU/EN translator device driver.
* It allows users to set the translation mode (Russian to English or English to Russian) via an ioctl command,
* write sentences to the device for translation, and read back the translated sentences.
**************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdint.h>

#define DEVICE "/dev/ruen_translator"
#define IOCTL_SET_MODE _IOW('t', 'm', int32_t*)

int main() { 
    int32_t mode;
    char buffer[256];
    int fd;

    fd = open(DEVICE, O_RDWR);
    if (fd < 0) {
        perror("Failed to open the device");
        return 1;
    }

    // Set mode to RU to EN
    printf("Enter 0 for RU->EN or 1 for EN->RU: ");
    if(scanf("%d", &mode) != 1) {
        printf("Invalid input\n");
        close(fd);
        return 1;
    }

    if(ioctl(fd, IOCTL_SET_MODE, &mode) < 0) {
        perror("Failed to set mode");
        close(fd);
        return 1;
    }

    // Clear newline left by scanf
    getchar(); 

    // Get the sentence
    printf("Enter a sentence to translate: ");
    if(fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("Failed to read input\n");
        close(fd);
        return 1;
    }

    // Remove newline character from fgets
    buffer[strcspn(buffer, "\n")] = 0;

    // Write to device
    write(fd, buffer, strlen(buffer));

    // Read from device
    memset(buffer, 0, sizeof(buffer));
    read(fd, buffer, sizeof(buffer) - 1);

    printf("Translated sentence: %s\n", buffer);

    close(fd);
    return 0;
}