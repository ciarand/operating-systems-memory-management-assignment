#include "process.h"
#include "queue.h"
#include "memory.h"

// creates a list of processes based on the provided filePath
PROCESS* assignProcessList(const char* file_path);

// returns a string (T = x) or a tab (\t) based on whether anything's been
// announced this clock tick
char* get_announcement_prefix(int current_time);

// gets the number of processes (the first number in the provided file)
int getNumProcess(FILE* filePtr);

// the main function
int main();

// asks for numeric input from the user (using the output param) and validates
// it with the provided function pointer
int processNumericInputFromUser(const char* output, int (*func)(int));

// initializes the provided frame_list based on the other params
void assignFrameList(frame_list* list, int page_size, int num_frames);

// assigns any available memory to waiting procs that'll fit in there
void assign_available_memory_to_waiting_procs(int current_time);

// clears any extra chars from stdin
void clearStdin(char* buf);

// removes any completed procs from memory
void dequeue_completed_procs(int current_time);

// adds any newly arrived procs to the input queue
void enqueue_newly_arrived_procs(int current_time);

// gets the user input (mem size, number of pages, path to input file)
void get_user_input(int* mem, int* page, char* file_path);

// the main program loop
void main_loop();

// prints the average turnaround time
void print_turnaround_times();

// prompts the user for a valid filename
void prompt_for_filename(char* res);
