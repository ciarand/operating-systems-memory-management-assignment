//
//  main.c
//  Project3
//
//  Created by Stratton Aguilar on 7/2/14.
//  Copyright (c) 2014 Stratton Aguilar. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "process.h"

// global constants
const int TIME_MAX = 100000;

// function prototypes
void GetInput(int *mem, int * page);
int GetNumProcess(const char* fileName);
void assignProcessList(const char* filePath, PROCESS* processList, int numProc);
void assignFrameList(FRAME* frameList, int pageSize, int numPage);

int main(int argc, const char * argv[]) {
    int pageSize = 0;
    int memSize = 0;
    int numPage = 0;
    int numProcess = 0;
    // absolute path to in1.txt
    char* filePath = "/Users/rabbitofalice/Documents/CSUF/CSUF Summer 2014/CPSC 351/Project_3/in1.txt";

    GetInput(&memSize, &pageSize);

    numProcess = GetNumProcess(filePath);

    // allocate space for process array
    PROCESS* processlist = malloc(numProcess * sizeof(PROCESS));

    //assign values to processes from file
    assignProcessList(filePath, processlist, numProcess);

    // get number of pages
    numPage = memSize / pageSize;

    // create frames
    FRAME* framelist = malloc(numPage * sizeof(FRAME));
    // assign frames
    assignFrameList(framelist, pageSize, numPage);

    return 0;
}

// prompts for memory size and page size
void GetInput(int* mem, int* page) {
    bool getCorrectData = true;

    while (getCorrectData) {
        printf("Memory Size : ");
        scanf("%d", mem);
        while (*page < 1 || *page > 3) {
            printf("Page Size (1: 100, 2: 200, 3: 400) : ");
            scanf("%d", page);
            if (*page < 1 || *page > 3) {
                printf("Invalid entry!\n");
            }
        }

        switch (*page) {
        case 1:
            *page = 100;
            break;
        case 2:
            *page = 200;
            break;
        case 3:
            *page = 400;
            break;
        default:
            *page = 100;
            break;
        }

        if ((*mem) % (*page) == 0) {
            getCorrectData = false;
        }else  {
            printf("Error: Memory Size must be a multiple of the page!\nPlease retry.\n");
        }
    }
    return;
}

// get number of processes from file
int GetNumProcess(const char* fileName) {
    FILE* filePtr;
    int numProc = 0;

    filePtr = fopen(fileName, "r");
    if (!filePtr) {
        printf("ERROR: Failed to open file %s", fileName);
    }else  {
        fscanf(filePtr, "%d", &numProc);
    }
    return numProc;
}

//stores values processes in process array
void assignProcessList(const char* filePath, PROCESS* processList, int numProc) {
    FILE* filePtr;
    int counter = 0;
    int numSpace;
    char buf[60];
    int totalSpace = 0;
    int temp;
    filePtr = fopen(filePath, "r");

    if (!filePtr) {
        printf("ERROR: Failed to open file %s", filePath);
    }else  {
        while (!feof(filePtr) && counter < numProc) {
            // clear first line
            fgets(buf, 60, filePtr);

            // store values for processes
            fscanf(filePtr, "%d %d %d %d", &(processList[counter].processNum), &(processList[counter].arrivalTime), &(processList[counter].lifeTime), &numSpace);

            // get total memory requirements for process
            totalSpace = 0;
            for (int i = 0; i < numSpace; i++) {
                fscanf(filePtr, "%d", &temp);
                totalSpace += temp;
            }
            processList[counter].memReq = totalSpace;

            // for testing
            printf("%d %d %d %d\n", (processList[counter].processNum), (processList[counter].arrivalTime), (processList[counter].lifeTime), processList[counter].memReq);
            // increment for next process
            counter++;
        }
    }
    fclose(filePtr);
    return;
}

// assigns frames to memory and sets assigned to process to false
void assignFrameList(FRAME* frameList, int pageSize, int numPage) {
    int value = 0;
    char starting[40];
    char ending[20];

    // assigne the frame list to empty set the memory
    for (int i = 0; i < numPage; i++) {
        frameList[i].assigned = false;

        sprintf(starting, "%d", value);
        strcat(starting, "-");
        //setting value inclusive starting from zero
        value += pageSize - 1;

        sprintf(ending, "%d", value);
        strcat(starting, ending);
        strcpy(frameList[i].location, starting);

        value++;

        // for testing
        printf("%s\n", frameList[i].location);
    }
}