/**
* @author - Hugh Hui
* @file milestone4.h -  This header file declares the methods in the milestone4.cpp file.
* 1/31/2025 -  H. Hui created file and added comments.
* 2/1/2025 - H. Hui added doxygen formatted comments
*/
#ifndef MILESTONE4_H
#define MILESTONE4_H

#include <string>

/**
 * @brief Logs a message to both the console and a file.
 *
 * This function writes the given message to the console using `std::cout` and
 * to a log file. It helps with logging important events, messages, or errors
 * for debugging or tracking purposes.
 *
 * @param message The message to be logged.
 *
 * This function is designed to be used across multiple cpp files for
 * consistent logging throughout the program.
 */
void logToFileAndConsole(const std::string& message);

#endif // MILESTONE4_H
