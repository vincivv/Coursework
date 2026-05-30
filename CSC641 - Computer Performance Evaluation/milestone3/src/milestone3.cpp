/**
*
* milestone3.cpp : This file contains the 'main' function. Program execution begins and ends there.
*
* 02/21/26 - Created by ChatGPT with prompt "write C++ program reads and parses the file: milestone3_config.json"
*            The file: "milestone3_config.json" is in the following format:
*
                {
                    "milestone3": [
                        {
                            "files": [
                                {
                                    "testcaseFile": "milestone3.json",
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
#include <thread>
#include <mutex>
#include <vector>
#include <atomic>
#include <numeric>
#include <iomanip>
#include <sstream>
#include <cmath>


#include "json.hpp"

using json = nlohmann::json;

#define _CRT_SECURE_NO_WARNINGS
#define CONFIG_FILE "milestone3_config.json"

// Global variable to be used for logging output
std::ofstream _outFile;

// Mutex to protect console/file output from concurrent threads
std::mutex _logMutex;

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
 * MethodStats
 * @brief Struct to hold statistics for a method being benchmarked
 */

struct MethodStats {
    std::vector<double> times;   // elapsed time (ms) for every call
    long long calls = 0;
};

// Five methods: 0=getItem, 1=add, 2=contains, 3=remove, 4=clear
static const int NUM_METHODS = 5;
static const char* METHOD_NAMES[NUM_METHODS] = {
    "getItem", "add", "contains", "remove", "clear"
};

/**
 * CallCounts
 *
 * @brief Holds the dynamically computed per-cycle call counts for each method.
 *
 * These values are derived at runtime from the JSON ratios so that changing
 * milestone3.json changes the benchmark behaviour without recompiling.
 *
 * getItemPerSlot  - number of getItem() calls per interleaved slot
 *                   (there are 3 slots per outer iteration, around add/contains/remove)
 * containsEvery   - call contains() once every N outer iterations;
 *                   0 means contains is not in the JSON / never called
 */
struct CallCounts {
    int getItemPerSlot;
    int containsEvery;
};


/**
 * buildDispatchTable
 * @brief Builds a dispatch table based on the provided ratios for each method.
 * The dispatch table is a vector of method indices where the frequency of each index corresponds to the specified ratio.
 * @param ratios An array of integers representing the ratio of calls for each method. The length of the array should be equal to NUM_METHODS.
 * @return A vector of integers representing the dispatch table, where each integer corresponds to a method index.
 */
std::vector<int> buildDispatchTable(const int ratios[NUM_METHODS]) {
    std::vector<int> table;
    for (int m = 0; m < NUM_METHODS; ++m) {
        for (int r = 0; r < ratios[m]; ++r) {
            table.push_back(m);
        }
    }
    return table;
}

/**
 *
 * computeCallCounts
 *
 * @brief Dynamically computes per-cycle call counts from the JSON ratios.
 * @param ratios    array of NUM_METHODS integer percentages read from JSON
 * @return          CallCounts with computed getItemPerSlot and containsEvery
 */
CallCounts computeCallCounts(const int ratios[NUM_METHODS]) {
    // Avoid divide-by-zero if add ratio is 0
    int addRatio = ratios[1] > 0 ? ratios[1] : 1;
 
    // getItem calls per interleaved slot, computed from ratio proportions
    int getItemPerSlot = (int)std::round(ratios[0] / (double)addRatio);
    if (getItemPerSlot < 1) getItemPerSlot = 1;
 
    // contains: call once every round(add% / contains%) outer iterations
    // e.g. add=9, contains=2 -> every round(4.5) = 4 or 5 iterations
    int containsEvery = (ratios[2] > 0)
        ? (int)std::round(addRatio / (double)ratios[2])
        : 0;
 
    return { getItemPerSlot, containsEvery };
}

/**
 * benchmarkThread
 * @brief Function executed by each thread in the benchmark. It performs a series of cache operations based on a dispatch table and records the time taken for each method.
 * @param cm Reference to the CacheManager being benchmarked.
 * @param dispatchTable A vector of method indices that determines which method to call in each iteration, based on the specified ratios.
 * @param testSize The number of items initially loaded into the cache, used to generate existing keys for hits.
 * @param testIterations The number of iterations (method calls) this thread should perform.
 * @param sleepIntervalMs Optional sleep interval in milliseconds between calls to throttle the call rate (can be set to 0 for no sleep).
 * @param threadId The unique ID of the thread, used for generating unique keys and values.
 * @param threadStats An array of MethodStats structures where this thread will record the time taken for each method call. The array should have a length of NUM_METHODS.
 */
void benchmarkThread(
    cache::CacheManager<int, std::string, bench::TbbBench>& cm,
    const CallCounts& cc,
    int testSize,
    int testIterations,
    int sleepIntervalMs,
    int threadId,
    MethodStats threadStats[NUM_METHODS]
) {
    std::mt19937 rng(std::random_device{}() ^ (threadId * 2654435761u));
    std::uniform_int_distribution<int> existingKeyDist(1, testSize);
 
    // Each thread gets its own unique new-key space so add/remove calls from
    // different threads never collide. Block is large enough for all iterations.
    int newKeyBase = testSize + threadId * (testIterations * (cc.getItemPerSlot + 2) + 1);
    int newKeyIdx  = 0;
 
    // Helper lambda: times a single cache call and records it in threadStats
    auto timeCall = [&](int methodIdx, auto fn) {
        auto t0 = std::chrono::high_resolution_clock::now();
        fn();
        auto t1 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = t1 - t0;
        threadStats[methodIdx].times.push_back(elapsed.count());
        threadStats[methodIdx].calls++;
    };
 
    // ── Outer loop: testIterations cycles ────────────────────────────────────
    //
    // Each cycle calls methods in the interleaved order from the professor's
    // pseudocode. The inner loop bounds are driven by CallCounts (computed
    // dynamically from JSON ratios), not hardcoded values.
    //
    for (int i = 1; i <= testIterations; ++i) {
        int existingKey = existingKeyDist(rng);         // key 1..testSize (in cache)
        int newKey      = newKeyBase + newKeyIdx++;     // key > testSize  (not in cache)
 
        // Slot 1: getItem – hits on random existing keys
        for (int j = 0; j < cc.getItemPerSlot; ++j) {
            int k = existingKeyDist(rng);
            timeCall(0, [&]{ cm.getItem(k); });
        }
 
        // add – insert a new key that is not in the cache
        timeCall(1, [&]{
            cm.add(newKey, "t" + std::to_string(threadId) + "_k" + std::to_string(newKey));
        });
 
        // Slot 2: getItem – mix of hits (existing) and misses (newly added key)
        for (int j = 0; j < cc.getItemPerSlot; ++j) {
            // Every 4th call in this slot tries the just-added key (likely a hit now)
            int k = (j % 4 == 0) ? newKey : existingKeyDist(rng);
            timeCall(0, [&]{ cm.getItem(k); });
        }
 
        // contains – called dynamically based on computed ratio
        // e.g. containsEvery=4 means called on iterations 4, 8, 12, ...
        if (cc.containsEvery > 0 && (i % cc.containsEvery == 0)) {
            int k = existingKeyDist(rng);
            timeCall(2, [&]{ cm.contains(k); });
        }
 
        // Slot 3: getItem – hits on random existing keys
        for (int j = 0; j < cc.getItemPerSlot; ++j) {
            int k = existingKeyDist(rng);
            timeCall(0, [&]{ cm.getItem(k); });
        }
 
        // remove – remove an existing key, then restore it so future
        // iterations can still find it as a cache hit
        timeCall(3, [&]{
            cm.remove(existingKey);
            cm.add(existingKey, "restored_t" + std::to_string(threadId));
        });
 
        // Optional sleep to throttle the call rate and avoid saturating the cache.
        // Start with zero; raise sleepInterval in the config if the cache gets saturated.
        if (sleepIntervalMs > 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(sleepIntervalMs));
        }
    }
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
    int testSize = config["Milestone3"][0]["defaultVariables"][0]["testSize"];

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
    // ── Read config ──────────────────────────────────────────────────────────
    int testSize            = config["Milestone3"][0]["defaultVariables"][0]["testSize"];
    int testIterations      = config["Milestone3"][0]["defaultVariables"][0]["testIterations"];
    int degreeOfParallelism = config["Milestone3"][0]["defaultVariables"][0]["degreeOfParallelism"];
    int sleepIntervalMs     = config["Milestone3"][0]["defaultVariables"][0]["sleepInterval"];
 
    // Read ratios from the testcaseFile (milestone3.json)
    std::string testcaseFilePath = config["Milestone3"][0]["files"][0]["testcaseFile"];
 
    std::ifstream tcFile(testcaseFilePath);
    if (!tcFile.is_open()) {
        std::cerr << "Error opening testcase file: " << testcaseFilePath << std::endl;
        return;
    }
    json tcConfig;
    tcFile >> tcConfig;
    tcFile.close();
 
    // Parse ratios dynamically from JSON — no hardcoded fallback values.
    // Changing the JSON changes the benchmark behaviour without recompiling.
    int ratios[NUM_METHODS] = {0};
    auto& ratioObj = tcConfig["CacheManagerBenchmark"][0]["testCase1"][0]["ratioOfMethods"][0];
    ratios[0] = ratioObj.contains("getItem")  ? ratioObj["getItem"].get<int>()  : 0;
    ratios[1] = ratioObj.contains("add")      ? ratioObj["add"].get<int>()      : 0;
    ratios[2] = ratioObj.contains("contains") ? ratioObj["contains"].get<int>() : 0;
    ratios[3] = ratioObj.contains("remove")   ? ratioObj["remove"].get<int>()   : 0;
    ratios[4] = 0; // clear is always 0 in-loop; performed once after all threads finish
 
    // Dynamically compute per-iteration call counts from the JSON ratios.
    // This is the key to the dynamic implementation:
    // the inner loop bounds change at runtime based on whatever is in the JSON.
    CallCounts cc = computeCallCounts(ratios);
 
    // ── Print configuration ───────────────────────────────────────────────────
    auto wallStart = std::chrono::system_clock::now();
    std::time_t startT = std::chrono::system_clock::to_time_t(wallStart);
    std::cout << "\nStarting computation at " << std::ctime(&startT);
 
    logToFileAndConsole("\nConfiguration for benchmark run:");
    logToFileAndConsole("\ttestSize:                  " + std::to_string(testSize));
    logToFileAndConsole("\ttestIterations:            " + std::to_string(testIterations));
    logToFileAndConsole("\tdegreeOfParallelism:       " + std::to_string(degreeOfParallelism));
    logToFileAndConsole("\tsleepInterval (ms):        " + std::to_string(sleepIntervalMs));
    logToFileAndConsole("\tgetItemPerSlot (computed): " + std::to_string(cc.getItemPerSlot));
    logToFileAndConsole("\tcontainsEvery  (computed): " + std::to_string(cc.containsEvery));
 
    std::string ratioLine = "\tRatios (from JSON): ";
    for (int m = 0; m < NUM_METHODS; ++m) {
        ratioLine += std::string(METHOD_NAMES[m]) + "=" + std::to_string(ratios[m]) + "% ";
    }
    logToFileAndConsole(ratioLine);
 
    // ── Allocate the Cache Manager ────────────────────────────────────────────
    cache::CacheManager<int, std::string, bench::TbbBench> cm;
 
    // ── Phase 1: Load the cache ───────────────────────────────────────────────
    auto loadStart = std::chrono::high_resolution_clock::now();
 
    for (int key = 1; key <= testSize; ++key) {
        std::string value = "Test value for key: " + std::to_string(key);
        cm.add(key, value);
        // logToFileAndConsole("Added key: " + std::to_string(key) + "; value: '" + value + "'");
    }
 
    auto loadEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> loadTime = loadEnd - loadStart;
 
    // Required output 1: time to do the initial load
    logToFileAndConsole("\nCache loaded with " + std::to_string(testSize)
                        + " items in " + std::to_string(loadTime.count()) + " ms.");
 
    // ── Phase 2: Multi-threaded benchmark ─────────────────────────────────────
    //
    // Each thread gets its own MethodStats[NUM_METHODS] array so there is no
    // shared mutable state during the benchmark — threads write only to their
    // own local stats and call the CacheManager (which is internally thread-safe
    // via TBB). The main thread aggregates after all threads finish.
    //
    std::vector<std::vector<MethodStats>> allThreadStats(
        degreeOfParallelism, std::vector<MethodStats>(NUM_METHODS));
 
    std::vector<std::thread> threads;
    threads.reserve(degreeOfParallelism);
 
    auto benchStart = std::chrono::high_resolution_clock::now();
 
    for (int t = 0; t < degreeOfParallelism; ++t) {
        threads.emplace_back(
            benchmarkThread,
            std::ref(cm),
            std::cref(cc),              // dynamically computed call counts from JSON ratios
            testSize,
            testIterations,
            sleepIntervalMs,
            t,
            allThreadStats[t].data()    // pointer to this thread's MethodStats[NUM_METHODS]
        );
    }
 
    // Wait for all threads to finish
    for (auto& thr : threads) {
        thr.join();
    }
 
    auto benchEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> benchTime = benchEnd - benchStart;
 
    logToFileAndConsole("\nAll threads completed in " + std::to_string(benchTime.count()) + " ms.");
 
    // ── Phase 3: Aggregate and print per-method statistics ────────────────────
    //
    // Required output:
    //   2. Total number of calls to each method aggregated across all threads
    //   3. Average time of each method aggregated across all threads
    //   4. Max time of each method aggregated across all threads
    //   5. Min time of each method aggregated across all threads
    //
    // Format helper: aligned columns matching the professor's output format
    auto fmtRow = [](const char* name, long long calls,
                     double avg, double maxT, double minT) -> std::string {
        std::ostringstream oss;
        oss << std::left  << std::setw(12) << name
            << std::right << std::setw(14) << calls
            << std::fixed << std::setprecision(6)
            << std::setw(14) << avg
            << std::setw(14) << maxT
            << std::setw(14) << minT;
        return oss.str();
    };
 
    logToFileAndConsole("\nPer-method statistics (aggregated across all threads):");
 
    // Print the header row
    {
        std::ostringstream hdr;
        hdr << std::left  << std::setw(12) << "Method"
            << std::right << std::setw(14) << "Total Calls"
            << std::setw(14) << "Avg (ms)"
            << std::setw(14) << "Max (ms)"
            << std::setw(14) << "Min (ms)";
        logToFileAndConsole(hdr.str());
    }
 
    for (int m = 0; m < NUM_METHODS; ++m) {
        // Collect all timing samples from every thread for this method
        std::vector<double> allTimes;
        long long totalCalls = 0;
        for (int t = 0; t < degreeOfParallelism; ++t) {
            auto& ms = allThreadStats[t][m];
            totalCalls += ms.calls;
            allTimes.insert(allTimes.end(), ms.times.begin(), ms.times.end());
        }
 
        if (allTimes.empty()) {
            std::ostringstream oss;
            oss << std::left  << std::setw(12) << METHOD_NAMES[m]
                << std::right << std::setw(14) << 0
                << std::setw(14) << "-"
                << std::setw(14) << "-"
                << std::setw(14) << "-";
            logToFileAndConsole(oss.str());
            continue;
        }
 
        double sum  = std::accumulate(allTimes.begin(), allTimes.end(), 0.0);
        double avg  = sum / static_cast<double>(allTimes.size());
        double maxT = *std::max_element(allTimes.begin(), allTimes.end());
        double minT = *std::min_element(allTimes.begin(), allTimes.end());
 
        logToFileAndConsole(fmtRow(METHOD_NAMES[m], totalCalls, avg, maxT, minT));
    }
 
    // ── Phase 4: Final clear ──────────────────────────────────────────────────
    // clear() is performed once here after all threads finish, not inside the
    // benchmark loop, to avoid wiping the shared cache mid-run.
    auto clearStart = std::chrono::high_resolution_clock::now();
    cm.clear();
    auto clearEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> clearTime = clearEnd - clearStart;
 
    // Required output 6: time to do the final clearing
    logToFileAndConsole("\nFinal clear (called once): " + std::to_string(clearTime.count()) + " ms.");
 
    // ── Total elapsed time ────────────────────────────────────────────────────
    auto wallEnd = std::chrono::system_clock::now();
    std::chrono::duration<double> totalElapsed = wallEnd - wallStart;
    std::time_t endT = std::chrono::system_clock::to_time_t(wallEnd);
 
    char endTimeBuf[64];
    std::strftime(endTimeBuf, sizeof(endTimeBuf), "%a %b %e %H:%M:%S %Y",
                  std::localtime(&endT));
 
    // Required output 7: total time of test run
    logToFileAndConsole("\nFinished computation at " + std::string(endTimeBuf));
    logToFileAndConsole("Number of Iterations (per thread): " + std::to_string(testIterations));
    logToFileAndConsole("Degree of Parallelism:             " + std::to_string(degreeOfParallelism));
    logToFileAndConsole("Total elapsed time:                " + std::to_string(totalElapsed.count()) + "s");
 
    // print out the cache manager results
    logToFileAndConsole("\nCache Manager statistics:");
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
    int testSize = config["Milestone3"][0]["defaultVariables"][0]["testSize"];
    int testIterations = config["Milestone3"][0]["defaultVariables"][0]["testIterations"];
 
    logToFileAndConsole("\nProcessing benchmark.");
    logToFileAndConsole("\n\nConfiguration for benchmark run:");
    logToFileAndConsole("\n\ttestSize: " + std::to_string(testSize));
    logToFileAndConsole("\ttestIterations: " + std::to_string(testIterations));
 
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
    std::string outputFilePath = config["Milestone3"][0]["files"][0]["outputFile"];
    std::string errorFilePath = config["Milestone3"][0]["files"][0]["errorLogFile"];

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
