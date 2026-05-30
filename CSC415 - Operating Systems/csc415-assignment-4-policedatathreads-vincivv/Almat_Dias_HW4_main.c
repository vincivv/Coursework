/**************************************************************
* Class::  CSC-415-03 Fall 2025
* Name::  Dias Almat
* Student ID::  923022045
* GitHub-Name::
* Project:: Assignment 4 – Processing FLR Data with Threads
*
* File:: template_HW4_main.c
*
* Description::  Main file, contains helper functions to convert date time strings to seconds,
                 structure to hold thread arguments, thread function to process records, function to 
                 print hash map stats to console. The main function first parses the field names and size
                 from the header.txt file, and stores them in an array of Field structures. It then reads the entire
                 record file into memory. The records are divided among the specified number of threads, each of which
                 processes its assigned records to compute the required statistics and store them in hash maps.
                 Finally, the computed statistics are printed to the console.
*
**************************************************************/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <Almat_Dias_HW4_hashMap.h>


//helper func to check if a year is leap year
int isLeap(int year) {
    if (year % 400 == 0){
        return 1;
    }
    if (year % 100 == 0){
        return 0;
    }
    return year % 4 == 0;
}


//helper func to get number of days in a month
int daysInMonth(int month, int year) {
    switch(month) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            return 31;
        case 4: case 6: case 9: case 11:
            return 30;
        case 2:
            return isLeap(year) ? 29 : 28;
        default:
            return 0;
    }
}


//calculate days since base time (1990-01-01)
long daysSinceBaseTime(int year, int month, int day) {
    const int baseYear = 1990;
    long days = 0;

    for(int y = baseYear; y < year; y++) {
        if(isLeap(y)) {
            days += 366;
        } else {
            days += 365;
        }
    }

    for(int m = 1; m < month; m++) {
        days += daysInMonth(m, year);
    }

    days += (day - 1);

    return days;
}


//convert datetime string to total seconds since base time
long convertToSeconds(const char *input){
    if(!input || !*input) return 0;

    int year, month, day, hour, minute, second;
    char ampm[3];

    month = (input[0] - '0') * 10 + (input[1] - '0');
    day = (input[3] - '0') * 10 + (input[4] - '0');
    year = (input[6] - '0') * 1000 + (input[7] - '0') * 100 + (input[8] - '0') * 10 + (input[9] - '0');
    hour = (input[11] - '0') * 10 + (input[12] - '0');
    minute = (input[14] - '0') * 10 + (input[15] - '0');
    second = (input[17] - '0') * 10 + (input[18] - '0');
    ampm[0] = input[20];
    ampm[1] = input[21];
    ampm[2] = '\0'; 

    if((ampm[0] == 'P' || ampm[0] == 'p') && hour != 12) {
        hour += 12;
    } else if((ampm[0] == 'A' || ampm[0] == 'a') && hour == 12) {
        hour = 0;
    }

    long totalDays = daysSinceBaseTime(year, month, day);
    long totalSeconds = totalDays * 86400 + hour * 3600 + minute * 60 + second;

    return totalSeconds;
}

//print hash map stats to console
void printHashToConsole(char *nord, char *title, HashMap *map, int which) {
    printf("\n%s  -  %s \n", nord, title);
    printf("%-30s | %8s | %6s | %6s | %6s | %6s | %8s | %6s | %6s | %6s | %6s | %8s\n",
           "CALL TYPE","COUNT","MIN","LB","Q1","MED","MEAN","Q3","UB","MAX","IQR","STDDEV");

    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode *node = map->slots[i];
        while (node) {
            Stats *s = NULL;

            // select which metric to print (0=DR, 1=SE, 2=SR)
            if (which == 0)
                s = &node->dr;
            else if (which == 1)
                s = &node->se;
            else
                s = &node->sr;

            if (s->count > 0 && !(s->min == 0 && s->max == 0 && s->mean == 0)) {
                printf("%-30s | %8ld | %6ld | %6ld | %6ld | %6ld | %8.2f | %6ld | %6ld | %6ld | %6ld | %8.2f\n",
                        node->key, s->count, s->min, s->lb, s->q1, s->median,
                        s->mean, s->q3, s->ub, s->max, s->iqr, s->stddev);    
            }
            node = node->next;
        }
    }
}

struct Field{
        char name[30];
        int width;
        int start;
};

//structure to hold thread arguments
typedef struct threadArguments{
    int fd;
    int threadID;
    long startRecord;
    long endRecord;
    int recordLength;
    struct Field *fields;
    int recDT, dispDT, onsceneDT, enrouteDT;
    int callTypeFinalDT, callTypeOriginalDT;
    int neighborhoodDT, districtDT;
    int useNeigbhborhood;
    char *neighborhoodOrDistrict1;
    char *neighborhoodOrDistrict2;
    long totalDRCount;
    long totalSECount;
    long totalSRCount;
    HashMap *totalMap;
    HashMap *subMap1;
    HashMap *subMap2;

    const char *fileBuf;
} threadArguments;

//thread function to process a range of records
static void *processThread(void *arg){

    //initialize local variables
    threadArguments *A = (threadArguments *)arg;
    char receive[26], dispatch[26], onscene[26], enroute[26], 
    callTypeFinal[26], callTypeOriginal[26], neighborhood[26], district[26];

    //process each record in the assigned range
    for(long i = A->startRecord; i < A->endRecord; i++) {
        int entry = i * A->recordLength;
        const char *recordBuffer = A->fileBuf + entry;

        int len;

        len = A->fields[A->recDT].width;
        memcpy(receive, recordBuffer + A->fields[A->recDT].start, len);
        receive[len] = '\0';

        len = A->fields[A->dispDT].width;
        memcpy(dispatch, recordBuffer + A->fields[A->dispDT].start, len);
        dispatch[len] = '\0';

        len = A->fields[A->onsceneDT].width;
        memcpy(onscene, recordBuffer + A->fields[A->onsceneDT].start, len);
        onscene[len] = '\0';

        len = A->fields[A->enrouteDT].width;
        memcpy(enroute, recordBuffer + A->fields[A->enrouteDT].start, len);
        enroute[len] = '\0';

        len = A->fields[A->callTypeFinalDT].width;
        memcpy(callTypeFinal, recordBuffer + A->fields[A->callTypeFinalDT].start, len);
        callTypeFinal[len] = '\0';

        len = A->fields[A->callTypeOriginalDT].width;
        memcpy(callTypeOriginal, recordBuffer + A->fields[A->callTypeOriginalDT].start, len);
        callTypeOriginal[len] = '\0';  

        callTypeFinal[0] = callTypeOriginal[0] = '\0';
        if(A->callTypeFinalDT >= 0){
            len = A->fields[A->callTypeFinalDT].width;
            memcpy(callTypeFinal, recordBuffer + A->fields[A->callTypeFinalDT].start, len);
            callTypeFinal[len] = '\0';
        }
        if(A->callTypeOriginalDT >= 0){
            len = A->fields[A->callTypeOriginalDT].width;
            memcpy(callTypeOriginal, recordBuffer + A->fields[A->callTypeOriginalDT].start, len);
            callTypeOriginal[len] = '\0';
        }
        char *callType = (callTypeFinal[0] != 0) ? callTypeFinal : callTypeOriginal;

        neighborhood[0] = district[0] = '\0';
        if(A->useNeigbhborhood && A->neighborhoodDT >= 0){
            len = A->fields[A->neighborhoodDT].width;
            memcpy(neighborhood, recordBuffer + A->fields[A->neighborhoodDT].start, len);
            neighborhood[len] = '\0';
        } else if(!A->useNeigbhborhood && A->districtDT >= 0){
            len = A->fields[A->districtDT].width;
            memcpy(district, recordBuffer + A->fields[A->districtDT].start, len);
            district[len] = '\0';
        }

        long sReceived = 0, sDispatch = 0, sOnscene = 0, sEnroute = 0;
        if(receive[0] != '\0') sReceived = convertToSeconds(receive);
        if(dispatch[0] != '\0') sDispatch = convertToSeconds(dispatch);
        if(onscene[0] != '\0') sOnscene = convertToSeconds(onscene);
        if(enroute[0] != '\0') sEnroute = convertToSeconds(enroute);

        int haveDR = (sReceived && sDispatch);
        int haveEO = (sOnscene && sEnroute);
        int haveOR = (sReceived && sOnscene);

        int dr = sDispatch - sReceived;
        int se = sOnscene - sEnroute;
        int sr = sOnscene - sReceived;

        if (haveDR) A->totalDRCount++;
        if (haveEO) A->totalSECount++;
        if (haveOR) A->totalSRCount++;

        if(callType[0]){
            addToHashMap(A->totalMap, callType, dr, haveDR, se, haveEO, sr, haveOR);
            //addToHashMap(A->totalMap, "TOTAL", dr, haveDR, se, haveEO, sr, haveOR);

            if(A->useNeigbhborhood){
                if (strcmp(neighborhood, A->neighborhoodOrDistrict1) == 0) {
                    addToHashMap(A->subMap1, callType, dr, haveDR, se, haveEO, sr, haveOR);
                    //addToHashMap(A->subMap1, "TOTAL",   dr, haveDR, se, haveEO, sr, haveOR);
                } else if (strcmp(neighborhood, A->neighborhoodOrDistrict2) == 0) {
                    addToHashMap(A->subMap2, callType, dr, haveDR, se, haveEO, sr, haveOR);
                    //addToHashMap(A->subMap2, "TOTAL",   dr, haveDR, se, haveEO, sr, haveOR);
                }
            } else { 
                if (strcmp(district, A->neighborhoodOrDistrict1) == 0) {
                    addToHashMap(A->subMap1, callType, dr, haveDR, se, haveEO, sr, haveOR);
                    //addToHashMap(A->subMap1, "TOTAL",   dr, haveDR, se, haveEO, sr, haveOR);
                } else if (strcmp(district, A->neighborhoodOrDistrict2) == 0) {
                    addToHashMap(A->subMap2, callType, dr, haveDR, se, haveEO, sr, haveOR);
                    //addToHashMap(A->subMap2, "TOTAL",   dr, haveDR, se, haveEO, sr, haveOR);
                }
            }
        }
    }
    return NULL;
}

int main (int argc, char *argv[])
    {
    //***TO DO***  Look at arguments, initialize application
    
    struct Field fields[37];
    int fieldCount = 0;
    int recordLength = 0;

    int headerFD = open(argv[2], O_RDONLY);
    struct stat fileStat;
    fstat(headerFD, &fileStat);
    int fileSize = fileStat.st_size;
    char *buffer = malloc(fileSize + 1);

    int bytesRead = read(headerFD, buffer, fileSize);
    buffer[bytesRead] = '\0';
    close(headerFD);

    //parse header file
    char *line = strtok(buffer, "\n");
    while(line && fieldCount < 37) {
        char *delim = strchr(line, ':');
        if(delim){
            *delim = '\0';
            int widthStr = atoi(line);
            char *nameStr = delim + 1;

            if(widthStr && nameStr) {
                int width = atoi(line);

                fields[fieldCount].width = width;
                fields[fieldCount].start = recordLength;
                strncpy(fields[fieldCount].name, nameStr, sizeof(fields[fieldCount].name) - 1);
                fields[fieldCount].name[sizeof(fields[fieldCount].name) - 1] = '\0';

                recordLength += width;
                fieldCount++;
            }
            line = strtok(NULL, "\n");
        } 
        
    }
    free(buffer);


    int recDT = -1, dispDT = -1, onsceneDT = -1, enrouteDT = -1;
    int callTypeFinalDT = -1, callTypeOriginalDT = -1;
    int neighborhoodDT = -1, districtDT = -1;

    //map field names to their indices
    for(int i = 0; i < fieldCount; i++) {
        const char *trimRecord = fields[i].name;
        while(*trimRecord == ' ' || *trimRecord == '\t') trimRecord++;

        if(strcmp(trimRecord, "received_datetime") == 0) {
            recDT = i;
        } else if(strcmp(trimRecord, "dispatch_datetime") == 0) {
            dispDT = i;
        } else if(strcmp(trimRecord, "onscene_datetime") == 0) {
            onsceneDT = i;
        } else if(strcmp(trimRecord, "enroute_datetime") == 0) {
            enrouteDT = i;
        } else if(strcmp(trimRecord, "call_type_final_desc") == 0) {
            callTypeFinalDT = i;
        } else if(strcmp(trimRecord, "call_type_original_desc") == 0) {
            callTypeOriginalDT = i;
        } else if(strcmp(trimRecord, "analysis_neighborhood") == 0) {
            neighborhoodDT = i;
        } else if(strcmp(trimRecord, "police_district") == 0) {
            districtDT = i;
        }
    }



    //**************************************************************
    // DO NOT CHANGE THIS BLOCK
    //Time stamp start
    struct timespec startTime;
    struct timespec endTime;

    clock_gettime(CLOCK_REALTIME, &startTime);
    //**************************************************************
    
    // *** TO DO ***  start your thread processing
    //                wait for the threads to finish



    int RecordFD = open(argv[1], O_RDONLY);
 
    struct stat recordFileStat;
    fstat(RecordFD, &recordFileStat);
    int recordFileSize = recordFileStat.st_size;
    int totalRecords = recordFileSize / recordLength;


    //determine if using neighborhood or district
    int useNeigbhborhood = 0;
    if(strcmp(argv[4], "analysis_neighborhood") == 0) {
        useNeigbhborhood = 1;
    } else{
        useNeigbhborhood = 0;
    }
    char *neighborhoodOrDistrict1 = argv[5];
    char *neighborhoodOrDistrict2 = argv[6];

    
    char receive[26], dispatch[26], onscene[26], enroute[26], 
    callTypeFinal[26], callTypeOriginal[26], neighborhood[26], district[26];

    int expectedBytes = totalRecords * recordLength;
    char *fileBuf = malloc(expectedBytes);

    //read entire file into the buffer
    int offset = 0;
    while(offset < expectedBytes) {
        int bytesRead = pread(RecordFD, fileBuf + offset, expectedBytes - offset, offset);
        offset += bytesRead;
    }
    close(RecordFD);




    //number of threads to create
    int numThreads = atoi(argv[3]);
    HashMap totalMap, subMap1, subMap2;
    initHashMap(&totalMap);
    initHashMap(&subMap1);
    initHashMap(&subMap2);

    //create threads
    pthread_t *threadIds = malloc(numThreads * sizeof(pthread_t));
    threadArguments *threadArgs = malloc(numThreads * sizeof(threadArguments));

    //distribute records among threads
    long recordsPerThread = totalRecords / numThreads;
    long remainingRecords = totalRecords % numThreads;
    long nextStartRecord = 0;

    //initialize and start each thread
    for(int t = 0; t < numThreads; t++) {
        long recordsForThisThread = recordsPerThread + (t < remainingRecords ? 1 : 0);
        threadArgs[t].threadID = t;

        threadArgs[t].fileBuf = fileBuf;

        threadArgs[t].startRecord = nextStartRecord;
        threadArgs[t].endRecord = nextStartRecord + recordsForThisThread;
        nextStartRecord += recordsForThisThread;

        threadArgs[t].recordLength = recordLength;
        threadArgs[t].fields = fields;

        threadArgs[t].recDT = recDT;
        threadArgs[t].dispDT = dispDT;
        threadArgs[t].onsceneDT = onsceneDT;
        threadArgs[t].enrouteDT = enrouteDT;
        threadArgs[t].callTypeFinalDT = callTypeFinalDT;
        threadArgs[t].callTypeOriginalDT = callTypeOriginalDT;
        threadArgs[t].neighborhoodDT = neighborhoodDT;
        threadArgs[t].districtDT = districtDT;

        threadArgs[t].useNeigbhborhood = useNeigbhborhood;
        threadArgs[t].neighborhoodOrDistrict1 = neighborhoodOrDistrict1;
        threadArgs[t].neighborhoodOrDistrict2 = neighborhoodOrDistrict2;

        threadArgs[t].totalDRCount = 0;
        threadArgs[t].totalSECount = 0;
        threadArgs[t].totalSRCount = 0;

        threadArgs[t].totalMap = &totalMap;
        threadArgs[t].subMap1 = &subMap1;
        threadArgs[t].subMap2 = &subMap2;

        pthread_create(&threadIds[t], NULL, processThread, &threadArgs[t]);
    }

    //wait for all threads to finish
    for(int t = 0; t < numThreads; t++) {
        pthread_join(threadIds[t], NULL);
    }

    finalizeHashMap(&totalMap);
    finalizeHashMap(&subMap1);
    finalizeHashMap(&subMap2);

    char *nord[3];
    if(useNeigbhborhood){
        nord[0] = "Total";
        nord[1] = neighborhoodOrDistrict1;
        nord[2] = neighborhoodOrDistrict2;
    } else{
        nord[0] = "Total";
        nord[1] = neighborhoodOrDistrict1;
        nord[2] = neighborhoodOrDistrict2;
    } 



    // ***TO DO *** Display Data


printHashToConsole(nord[0], "Dispatch to Received", &totalMap, 0);
printHashToConsole(nord[0], "Enroute to Onscene", &totalMap, 1);
printHashToConsole(nord[0], "Onscene to Received", &totalMap, 2);

printHashToConsole(nord[1], "Dispatch to Received", &subMap1, 0);
printHashToConsole(nord[1], "Enroute to Onscene", &subMap1, 1);
printHashToConsole(nord[1], "Onscene to Received", &subMap1, 2);

printHashToConsole(nord[2], "Dispatch to Received", &subMap2, 0);
printHashToConsole(nord[2], "Enroute to Onscene", &subMap2, 1);
printHashToConsole(nord[2], "Onscene to Received", &subMap2, 2);




    //**************************************************************
    // DO NOT CHANGE THIS BLOCK
    //Clock output
    clock_gettime(CLOCK_REALTIME, &endTime);
    time_t sec = endTime.tv_sec - startTime.tv_sec;
    long n_sec = endTime.tv_nsec - startTime.tv_nsec;
    if (endTime.tv_nsec < startTime.tv_nsec)
        {
        --sec;
        n_sec = n_sec + 1000000000L;
        }

    printf("Total Time was %ld.%09ld seconds for %s threads.\n", sec, n_sec, argv[3]);
    //**************************************************************


    // ***TO DO *** cleanup
    freeHashMap(&totalMap);
    freeHashMap(&subMap1);
    freeHashMap(&subMap2);
    free(threadIds);
    free(threadArgs);
    free(fileBuf);
}