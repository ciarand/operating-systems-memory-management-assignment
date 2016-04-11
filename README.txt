This project is deprecated and unmaintained. Proceed with caution!

Operating Systems project 3
===========================

Authors
-------
Ciaran Downey    - 891709818
Stratton Aguilar - 800768442

Programming language
--------------------
Written with pure C

Design
------
From a high level perspective this program consists of two main phases.

The first phase primarily happens in the `main` function in `main.c`. It
consists of setting up the relevant data structures, assigning memory, and
accepting and parsing user input (i.e. amount of memory, input file to read,
etc.).

The second phase is encapsulated in the `main_loop` function. That function is
essentially a loop that increments a counter (`current_time`, starting at 0)
until either the max time is reached or all processes are finished. Inside the
body of the loop the following actions are taken:

- Any newly arrived processes are put into a shared input queue

- Any completed processes are terminated and removed from memory

- Any waiting processes that can be fit into memory are

Finally, when the loop finishes the turnaround times are calculated and printed.

Data structures
---------------
There are 3 main data structures at work here.

The first is the `process` struct, that's essentially just a container for
related information. That includes facts like arrival time, total memory
required, time finished, etc.

The second is the `proc_queue` struct. This is, as the name would suggest,
a queue for processes waiting for memory to be allocated and assigned to them.
It's a standard circular queue (i.e. there's an index for both `rear` and
`front` values which allows us to loop backward). The only extra special feature
of this queue is its `dequeue_proc_at_index` function. This allows us to remove
a process from the given index, and the queue will naturally rearrange itself
afterward to preserve the correct order. This is essential as it may be the case
that the head element is too large to fit into memory while a later item can
easily be fit.

The third data structure is the memory map. This is implemented as a list
(`frame_list`) that contains an array of `FRAME`s, as well as some extra
information like the `page_size` and `number_of_frames`.

For each of these data structures (the `proc_queue` and the `frame_list`) there
are associated functions for printing their current status.

File structures
---------------
Utility functions related to the `proc_queue`, as well as the queue struct
itself are stored within `queue.h`. The `frame_list` and `FRAME` functions and
structs are stored in `memory.h`. Prototypes for the functions found in `main.c`
are stored within `prototypes.h`, which is also where the other two header files
are imported. `main.c` contains the main functions that take action, including
`main` and `main_loop`.

How to execute
--------------
Compiling the executable should be as simple as running `make`. It was tested
with `gcc-4.8.3`, but should work on any reasonably modern version of the
compiler that supports the c99 standard as well as the `-gstrictdwarf` flag.
That creates an executable named `main`, which is run with `./main` as you'd
expect. Once run, the program will prompt for 3 inputs:

```
Memory size: 2000
Page size (1: 100, 2: 200, 3: 400): 3
Input file: in1.txt
```

And then proceed to print the results of that run to stdout.

Other details
-------------
This was tested on an Arch Linux VM as well as Mac OS X 10.9.3 using a Homebrew
compiled GCC as well as the standard Xcode compiler (Clang).
