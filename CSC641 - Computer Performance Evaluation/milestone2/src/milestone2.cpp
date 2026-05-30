/**
*
* milestone2.cpp : This file contains the 'main' function. Program execution begins and ends there.
*
* 02/21/26 - Created by ChatGPT with prompt "write C++ program reads and parses the file: milestone2_config.json"
*            The file: "milestone2_config.json" is in the following format:
*
                {
                    "Milestone2": [
                        {
                            "files": [
                                {
                                    "testcaseFile": "milestone2.json",
                                    "outputFile": "generatedOutputFile.txt",
                                    "errorLogFile":"logFile.txt"
                                }
                            ],
                            "defaultVariables": [
                                {
                                    "numberOfIterations": 10000,
                                    "readSize": 100
                                }
                            ]
                        }
                    ]
                }
            and where the testcaseFile has the following format:
                {
                    "FileReadBenchmark": [
                        {
                            "testCase1": [
                                {
                                    "readTest": {"inputFileName": "top-output.txt", "numberOfIterations": 100}
                                }
                                ...
                            ]
                        }
                    ]
                }

2/15/2026 - create by Joseph Hui;

*/
#include "cache-manager.hpp"
#include "benchmark.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <random>

#include "json.hpp"

using json = nlohmann::json;

#define _CRT_SECURE_NO_WARNINGS
#define CONFIG_FILE "milestone2_config.json"

// Global variable to be used for logging output
std::ofstream _outFile;

/**
*
* getOutFile
*
* @brief function to return pointer to outFile
*
* @param        none
*
* @return       pointer to output file
*/
std::ofstream& getOutFile() {
    return _outFile;
}


/**
*
* setOutFile
*
* @brief function to set path and open output file
*
* @param filePath       the path to output file
*
* @return               nothing
*/
void setOutFile(const std::string& filePath) {
    // Close the current file if it's already open
    if (_outFile.is_open()) {
        _outFile.close();
    }

    // Open the new file
    _outFile.open(filePath);
    if (!_outFile.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
    }
}

/**
 * logToFileAndConsole
 *
 * @brief Logs a message to both the console and the output file.
 *
 * This helper function prints a message to the console and writes the same
 * message to the output file.
 *
 * @param message The message to log
 */
void logToFileAndConsole(std::string message) {
    // Get the output file
    std::ofstream& outFile = getOutFile();

    std::cout << message << std::endl;  // Print to console 
    outFile << message << std::endl;  // Write to file
}

/**
*
* getItemTest
*
* @brief calls CacheManager.getItem()
*
* @param    config              benchmark config
*
* @return   nothing, but output is sent to console and written to output file
*/
void getItemTest(json config, cache::CacheManager<int, std::string, bench::TbbBench> &cm) {
    int testSize = config["Milestone2"][0]["defaultVariables"][0]["testSize"];

    // generate a randome key to retrieve
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution distr(1, testSize);
    int testKey = distr(gen);

    // get the value from the Cache Manager for testKey
    // (this is the actual call for the test)
    auto val = cm.getItem(testKey);

    /*
     * Uncomment for some debug messages
    if (val) {
       logToFileAndConsole("Retrieved key: " + std::to_string(testKey) + "; with value: '" + *val + "'");
    } else {
       logToFileAndConsole("Key: " + std::to_string(testKey) + " not found (NOT EXPECTED!!!!");
    }
    */

    // try one that is not in the cache
    testKey = testKey * 1000;
    val = cm.getItem(testKey);
    /*
     * Uncomment for some debug messages
    if (val) {
       logToFileAndConsole("Retrieved key: " + std::to_string(testKey) + "; with value (NOT EXPECTED!!!): " + *val);
    } else {
       logToFileAndConsole("Key: " + std::to_string(testKey) + " not found (expected)!");
    }
    */
}

/**
*
* timeWrapper 
* 
* @brief Simple wrapper function that times a limited number of functions
*
* @param    config              benchmark config
*
* @return   nothing, but output is sent to console and written to output file
*/
void timeWrapper(json config) {
    // set the start time
    auto start = std::chrono::system_clock::now();
    std::time_t start_time = std::chrono::system_clock::to_time_t(start);

    // output some helpful comments to the console
    std::cout << "\nStarting computation at " << std::ctime(&start_time);

    // need to write out the data for each timed iteration in the following format:
    // 
    // iter#    end time    elapsed time     
    // 
    // 1        <time1>     10
    // 2        <time2>     9
    // 3        <time3>     11
    // ...
    //
    // aggregated statistics:
    //
    // avg      min         max
    //
    // 10       9           11

    int testSize = config["Milestone2"][0]["defaultVariables"][0]["testSize"];
    int testIterations = config["Milestone2"][0]["defaultVariables"][0]["testIterations"];
    
    // Allocate the cache manager
    cache::CacheManager<int, std::string, bench::TbbBench> cm;

    //set the start time for loading the cache
    auto loadStart = std::chrono::high_resolution_clock::now();

    // sample test load of the cache
    for (auto key = 1; key <= testSize; ++key) {
        std::string value = "Test value for key: " + std::to_string(key);
        cm.add(key, value);
        // logToFileAndConsole("Added key: " + std::to_string(key) + "; value: '" + value + "'");
    }

    auto loadEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> loadTime = loadEnd - loadStart;
    logToFileAndConsole("Cache loaded with " + std::to_string(testSize) + " items in " + std::to_string(loadTime.count()) + " ms.");

    // write out the head line for output file
    logToFileAndConsole("\niter#\t\tend time\telapsed time");

    // vector to hold the elapsed times for each iteration (for aggregated statistics)
    std::vector<double> times; 

    // call the specific function to time
    for (int i = 1; i < testIterations + 1; i++) {
        auto iterStart = std::chrono::high_resolution_clock::now(); //Start time for the iteration

        // float average = 0.0;
        // float min = 0.0;
        // float max = 0.0;
        //float elapsed = 0.0;

        // add more functions here
        int existingKey = (i % testSize) + 1; // This will ensure we get keys that are in the cache

        //new key that is not in the cache
        int newKey = testSize + i; // This will ensure we get keys that are not in the cache

        //1. getItem hit
        cm.getItem(existingKey);

        //2. getItem miss
        cm.getItem(newKey);

        //3. contains (should be false)
        bool conatainsBefore = cm.contains(newKey);
        if(conatainsBefore) {
            logToFileAndConsole("Unexpected: Cache contains key: " + std::to_string(newKey) + " before adding it!");
        }

        //4. add new item
        cm.add(newKey, "Test value for key: " + std::to_string(newKey));

        //5. contains (should be true)
        bool containsAfter = cm.contains(newKey);
        if(!containsAfter) {
            logToFileAndConsole("Unexpected: Cache does not contain key: " + std::to_string(newKey) + " after adding it!");
        }

        //6. contains existing key (shoild be true)
        if(!cm.contains(existingKey)) {
            logToFileAndConsole("Unexpected: Cache does not contain existing key: " + std::to_string(existingKey) + "!");
        }

        //7. remove existing key
        cm.remove(existingKey);

        //8. contains existing key (should be false)
        if(cm.contains(existingKey)) {
            logToFileAndConsole("Unexpected: Cache still contains existing key: " + std::to_string(existingKey) + " after removing it!");
        }

        //9 add existing key back
        cm.add(existingKey, "restored value");


        // write out the current values for this iteration
        auto curIterEnd = std::chrono::high_resolution_clock::now();

        //elapsed time for this iteration in milliseconds
        std::chrono::duration<double, std::milli> elapsed_seconds = curIterEnd - iterStart;
        double elapsed = elapsed_seconds.count();

        // add the elapsed time to the vector for aggregated statistics
        times.push_back(elapsed);

        // real end time in hh:mm:ss
        auto endTimeT = std::chrono::system_clock::now();
        std::time_t end_time = std::chrono::system_clock::to_time_t(endTimeT);
        std::tm* localTime = std::localtime(&end_time);

        char timeBuffer[9]; // Buffer to hold the formatted time (hh:mm:ss)
        std::strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S", localTime);
        std::string timeString(timeBuffer);


        logToFileAndConsole(std::to_string(i) + "\t\t" + timeString + "\t" + std::to_string(elapsed));
    }

    // output the aggregated statistics
    double sum = 0.0;
    double min = times[0];
    double max = times[0]; 

    // calculate the sum, min and max
    for(double t : times) {
        sum += t;
        if(t < min) {
            min = t;
        }
        if(t > max) {
            max = t;
        }
    }
    double avg = sum / times.size();

    logToFileAndConsole("\nAggregated statistics:");
    logToFileAndConsole("avg\t\tmin\t\tmax");
    logToFileAndConsole(
        std::to_string(avg) + "\t" + 
        std::to_string(min) + "\t" + 
        std::to_string(max)
    );

    //set the start time for clearing the cache
    auto clearStart = std::chrono::high_resolution_clock::now();
    cm.clear();
    auto clearEnd = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> clearTime = clearEnd - clearStart;

    logToFileAndConsole("\nCache cleared in " + std::to_string(clearTime.count()) + " ms.");

    // set the end time
    auto finalEnd = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = finalEnd - start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(finalEnd);

    std::cout << "\nFinished computation at " << std::ctime(&end_time)
        << "\nNumber of Iterations: " << testIterations
        << "\nElapsed time: " << elapsed_seconds.count() << "s";

    // print out the cache manager results
    std::cout << "\n\nCache Manager statistics:\n";
    auto benchmarkResults = cm.benchmark();
    bench::printBenchmark(benchmarkResults);

    return;
}


/**
*
* runBenchmark
*
* @brief run the benchmark
*
* @param    config          json configuration
*
* @return                   nothing, but output is sent to console and written to output file
*/
void runBenchmark(json config) {
    // Retrieve configured test cache size and iterations
    int testSize = config["Milestone2"][0]["defaultVariables"][0]["testSize"];
    int testIterations = config["Milestone2"][0]["defaultVariables"][0]["testIterations"];

    logToFileAndConsole("\nProcessing benchmark.");
    logToFileAndConsole("\n\nConfiguration for benchmark run:");
    logToFileAndConsole("\n\ttestSize: " + std::to_string(testSize));
    logToFileAndConsole("\ttestIterations: " + std::to_string(testIterations));

    // call to the timing wrapper (i.e., the actual test)
    timeWrapper(config);

    logToFileAndConsole("\nFinished processing benchmark.");
}

/**
*
* main
*
* main function which does the following:
*   read config file for input file, output file, error file, hash table size and FIFO size
*   create a hash table
*   for each of the test case
*       process test cases - display results to console and write to output file
*       print out the hash table
*       clear out hash table
*
* @param    none
*
* @return   nothing, but output is written to console and files
*/

int main() {
    // Load the config file
    std::ifstream configFile(CONFIG_FILE);
    if (!configFile.is_open()) {
        std::cerr << "Error opening config file!" << std::endl;
        return 1;
    }

    json config;
    configFile >> config;
    configFile.close();

    // Retrieve file paths from the config
    std::string outputFilePath = config["Milestone2"][0]["files"][0]["outputFile"];
    std::string errorFilePath = config["Milestone2"][0]["files"][0]["errorLogFile"];

    // Open up the outfile and set the output file path using the setter
    //
    // Treating output file differently than input and config files
    setOutFile(outputFilePath);
    std::ofstream& outFile = getOutFile();

    // Call the benchmark
    runBenchmark(config);

    logToFileAndConsole("\n\nEnd of tests.");

    // Close output file
    outFile.close();

    return 0;
}
