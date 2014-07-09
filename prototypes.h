//
//  queue.h
//  Project3
//
//  Authors:
//  - Stratton Aguilar
//  - Ciaran Downey
//
//  Some rights reserved. See the included LICENSE file.
//

#include "process.h"
#include "queue.h"
#include "memory.h"

// creates a list of processes based on the provided filePath
PROCESS* assign_process_list(const char* file_path);

// returns a string (T = x) or a tab (\t) based on whether anything's been
// announced this clock tick
char* get_announcement_prefix(int current_time);

// gets the number of processes (the first number in the provided file)
int get_number_of_processes_from_file(FILE* filePtr);

// the main function
int main();

// asks for numeric input from the user (using the output param) and validates
// it with the provided function pointer
int process_numeric_input_from_user(const char* output, int (*func)(int));

// assigns any available memory to waiting procs that'll fit in there
void assign_available_memory_to_waiting_procs(int current_time);

// clears any extra chars from stdin
void clear_stdin(char* buf);

// removes any completed procs from memory
void terminate_completed_procs(int current_time);

// adds any newly arrived procs to the input queue
void enqueue_newly_arrived_procs(int current_time);

// gets the user input (mem size, number of pages, path to input file)
void get_user_input(int* mem, int* page, char* file_path);

// returns a 1 if the provided int (t) is 1, 2, or 3 and a 0 otherwise
int is_one_two_or_three(int t);

// the main program loop
void main_loop();

// returns a 1 if the provided int (t) is a multiple of 100 and a 0 otherwise
int multiple_of_one_hundred(int t);

// prints the average turnaround time
void print_turnaround_times();

// prompts the user for a valid filename
void prompt_for_filename(char* res);
