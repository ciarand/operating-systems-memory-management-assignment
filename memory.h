typedef struct FRAME {
    // 1 if assigned to process, otherwise 0
    int assigned;
    char location[40];
    int procAssign;
    int pageNumber;
} FRAME;

typedef struct frame_list {
    FRAME* frames;
    int number_of_frames;
    int page_size;
} frame_list;

frame_list* create_frame_list(int number_of_frames, int page_size) {
    int i;

    frame_list *f;

    f = malloc(sizeof(frame_list));

    f->frames = (FRAME*)malloc(sizeof(FRAME) * number_of_frames);
    f->page_size = page_size;
    f->number_of_frames = number_of_frames;

    for (i = 0; i < f->number_of_frames; i += 1) {
        f->frames[i].assigned = 0;
    }

    return f;
}

int proc_can_fit_into_memory(frame_list* list, PROCESS* proc) {
    int i, num_free_frames = 0;

    for (i = 0; i < list->number_of_frames; i += 1) {
        if (!list->frames[i].assigned) {
            num_free_frames += 1;
        }
    }

    // if the number of free frames * the page size is greater than the mem req
    // for the process in question we can fit it in.
    return (num_free_frames * list->page_size) >= proc->mem_reqs;
}

void fit_proc_into_memory(frame_list* list, PROCESS* proc) {
    // this assumes you've already checked that you *can* fit the proc into mem
    int i, remaining_mem, current_page = 1;

    remaining_mem = proc->mem_reqs;

    for (i = 0; i < list->number_of_frames; i += 1) {
        // if this frame is not assigned
        if (!list->frames[i].assigned) {
            // assign it
            list->frames[i].assigned = 1;
            // set the page number
            list->frames[i].pageNumber = current_page;
            // set the proc num
            list->frames[i].procAssign = proc->pid;

            current_page++;
            remaining_mem -= list->page_size;
        }

        if (remaining_mem <= 0) {
            break;
        }
    }
}

void print_frame_list(frame_list* list) {
    int i, in_free_block = 0, start;

    printf("\tMemory map:\n");

    for (i = 0; i < list->number_of_frames; i += 1) {
        if (!in_free_block && !list->frames[i].assigned) {
            in_free_block = 1;
            start = i;
        } else if (in_free_block && list->frames[i].assigned) {
            in_free_block = 0;
            printf("\t\t%d-%d: Free frame(s)\n",
                   start * list->page_size,
                   ((i + 1) * list->page_size) - 1);
        }

        if (list->frames[i].assigned) {
            printf("\t\t%d-%d: Process %d, Page %d\n",
                   i * list->page_size,
                   ((i + 1) * list->page_size) - 1,
                   list->frames[i].procAssign,
                   list->frames[i].pageNumber);
        }
    }

    if (in_free_block) {
        printf("\t\t%d-%d: Free frame(s)\n",
               start * list->page_size,
               ((i) * list->page_size) - 1);
    }
}

int frame_list_is_empty(frame_list* list) {
    int i;

    for (i = 0; i < list->number_of_frames; i += 1) {
        if (list->frames[i].assigned) {
            return 0;
        }
    }

    return 1;
}

void free_memory_for_pid(frame_list* list, int pid) {
    int i;

    FRAME* frame;

    for (i = 0; i < list->number_of_frames; i += 1) {
        frame = &list->frames[i];

        if ((*frame).procAssign == pid) {
            (*frame).procAssign = 0;
            (*frame).pageNumber = 0;
            (*frame).assigned = 0;
        }
    }
}

