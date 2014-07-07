//
//  main.c
//  Project3
//
//  Authors:
//  - Stratton Aguilar
//  - Ciaran Downey
//
//  Some rights reserved. See the included LICENSE file.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "process.h"
#include "queue.h"
#include "memory.h"

// global constants
const int TIME_MAX = 100000;

// function prototypes
void getInput(int *mem, int * page);
int getNumProcess(FILE* filePtr);
PROCESS* assignProcessList(const char* filePath);
void assignFrameList(frame_list* list, int page_size, int num_frames);

// shared data
int number_of_procs = 0, last_announcement = -1;
PROCESS* proc_list;
proc_queue* queue;
frame_list* framelist;

char* get_announcement_prefix(int current_time) {
    char* result;

    result = malloc(20 * sizeof(char));

    if (last_announcement == current_time) {
        sprintf(result, "\t");
    } else {
        sprintf(result, "t = %d: ", current_time);
    }

    last_announcement = current_time;

    return result;
}

void main_loop() {
    int i, index;
    long current_time = 0;
    PROCESS* proc;

    while (1) {
        // queue any procs that have arrived
        for (i = 0; i < number_of_procs; i += 1) {
            proc = &proc_list[i];

            if (proc->arrivalTime == current_time) {
                printf("%sProcess %d arrives\n",
                        get_announcement_prefix(current_time),
                        proc->processNum);

                proc->is_active = 1;
                proc->time_left = proc->lifeTime;

                enqueue_proc(queue, proc);

                print_proc_queue(queue);
            }
        }

        // decrement all currently active procs (and dequeue them if we can)
        for (i = 0; i < number_of_procs; i += 1) {
            proc = &proc_list[i];
            if (proc->is_active) {
                proc->time_left--;

                if (proc->time_left <= 0) {
                    printf("%sProcess %d completes\n",
                            get_announcement_prefix(current_time),
                            proc->processNum);

                    proc->is_active = 0;

                    free_memory_for_pid(framelist, proc->processNum);

                    print_frame_list(framelist);
                }
            }
        }

        // enqueue any procs that can be put into mem
        for (i = 0; i < queue->size; i += 1) {
            index = iterate_queue_index(queue, i);
            proc = peek_queue_at_index(queue, index);

            if (proc_can_fit_into_memory(framelist, proc)) {
                printf("%sMM moves Process %d to memory\n",
                        get_announcement_prefix(current_time),
                        proc->processNum);

                fit_proc_into_memory(framelist, proc);
                dequeue_proc_at_index(queue, i);

                print_proc_queue(queue);
                print_frame_list(framelist);
            }
        }

        current_time++;

        if (current_time > TIME_MAX) {
            printf("max time reached\n");
            break;
        }

        if (queue->size == 0 && frame_list_is_empty(framelist)) {
            printf("all jobs finished\n");
            break;
        }
    }

    printf("all done\n");
}

int main() {
    int page_size = 0;
    int mem_size = 0;
    int num_frames = 0;

    char* filePath = "./in1.txt";
    // assign values to processes from file
    proc_list = assignProcessList(filePath);
    queue = create_proc_queue(number_of_procs);

    getInput(&mem_size, &page_size);

    // get number of frames
    num_frames = mem_size / page_size;
    // create framelist
    framelist = create_frame_list(num_frames, page_size);
    // assign frames
    // assignFrameList(framelist, page_size, num_frames);

    main_loop();

    return 0;
}

int multipleOfOneHundred(int t) {
    return (t % 100) == 0 ? 1 : 0;
}

int isOneTwoOrThree(int t) {
    return (t >= 1 && t <= 3) ? 1 : 0;
}

void clearStdin(char* buf) {
    if (buf[strlen(buf) - 1] != '\n') {
        int ch;
        while (((ch = getchar()) != '\n') && (ch != EOF)) ;
    }
}

int processNumericInputFromUser(const char* output, int (*func)(int)) {
    char buf[10];
    int success = 0;
    int res = 0;

    while (!success) {
        printf("%s: ", output);

        if (fgets(buf, 10, stdin) == NULL) {
            clearStdin(buf);
            printf("ERROR: You didn't enter any data!\n");

            continue;
        }

        if (sscanf(buf, "%d", &res) <= 0) {
            clearStdin(buf);
            printf("ERROR: You didn't enter a number!\n");

            continue;
        }

        if (!(success = (*func)(res))) {
            clearStdin(buf);
            printf("ERROR: That number is not a valid choice\n");
        }
    }

    return res;
}

// prompts for memory size and page size
void getInput(int* mem, int* page) {
    while (1) {
        *mem = processNumericInputFromUser(
            "Memory size", multipleOfOneHundred);

        *page = processNumericInputFromUser(
            "Page size (1: 100, 2: 200, 3: 400)", isOneTwoOrThree);

        switch (*page) {
        case 1: *page = 100; break;
        case 2: *page = 200; break;
        case 3: *page = 400; break;
        }

        if ((*mem) % (*page) == 0) {
            break;
        }

        printf("ERROR: Memory size must be a multiple of the page!");
        printf(" %d is not a multiple of %d, please retry.\n", *mem, *page);
    }
}

// get number of processes from file
int getNumProcess(FILE* filePtr) {
    int num = 0;

    fscanf(filePtr, "%d", &num);

    return num;
}

// stores values processes in process array
PROCESS* assignProcessList(const char* filePath) {
    int numSpace;
    int tmp;
    int counter = 0;
    int totalSpace = 0;
    FILE* filePtr = fopen(filePath, "r");

    number_of_procs = getNumProcess(filePtr);

    // allocate space for process array
    PROCESS* procList = malloc(number_of_procs * sizeof(PROCESS));

    if (!filePtr) {
        printf("ERROR: Failed to open file %s", filePath);
        exit(1);
    }

    while (!feof(filePtr) && counter < number_of_procs) {
        // store values for processes
        fscanf(filePtr, "%d %d %d %d",
               &(procList[counter].processNum),
               &(procList[counter].arrivalTime),
               &(procList[counter].lifeTime),
               &numSpace);

        // get total memory requirements for process
        totalSpace = 0;
        for (int i = 0; i < numSpace; i++) {
            fscanf(filePtr, "%d", &tmp);
            totalSpace += tmp;
        }
        procList[counter].memReq = totalSpace;

        procList[counter].is_active = 0;
        procList[counter].time_left = 0;

        counter++;
    }

    fclose(filePtr);

    return procList;
}

// assigns frames to memory and sets assigned to process to false
void assignFrameList(frame_list* list, int page_size, int num_frames) {
    int value = 0;
    char starting[40];
    char ending[20];

    // assign the frame list to empty set the memory
    for (int i = 0; i < num_frames; i++) {
        list->frames[i].assigned = 0;

        sprintf(starting, "%d", value);
        strcat(starting, "-");
        // setting value inclusive starting from zero
        value += page_size - 1;

        sprintf(ending, "%d", value);
        strcat(starting, ending);
        strcpy(list->frames[i].location, starting);

        value++;

        // for testing
        printf("%s\n", list->frames[i].location);
    }
}

