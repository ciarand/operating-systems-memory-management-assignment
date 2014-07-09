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

typedef struct proc_queue {
    int capacity;
    int size;
    int front;
    int rear;
    PROCESS **elements;
} proc_queue;

proc_queue* create_proc_queue(int length) {
    proc_queue *q;

    q = malloc(sizeof(proc_queue));

    q->elements = malloc(sizeof(PROCESS) * length);
    q->size = 0;
    q->capacity = length;
    q->front = 0;
    q->rear = -1;

    return q;
}

void enqueue_proc(proc_queue* q, PROCESS* proc) {
    if (q->size == q->capacity) {
        printf("ERROR: queue is full to capacity!\n");

        exit(2);
    }

    q->size++;
    q->rear = q->rear + 1;

    if (q->rear == q->capacity) {
        q->rear = 0;
    }

    q->elements[q->rear] = proc;
}

int queue_has_next(proc_queue* q) {
    return q->size == 0 ? 0 : 1;
}

PROCESS* peek_queue(proc_queue* q) {
    if (!queue_has_next(q)) {
        printf("ERROR: queue is empty, can't get the next element\n");
        exit(2);
    }

    return q->elements[q->front];
}

PROCESS* peek_queue_at_index(proc_queue* q, int index) {
    return q->elements[index];
}

void dequeue_proc(proc_queue* q) {
    if (!queue_has_next(q)) {
        printf("ERROR: queue is empty, can't dequeue anything\n");
        exit(2);
    }

    q->size--;
    q->front++;

    if (q->front == q->capacity) {
        q->front = 0;
    }
}

int iterate_queue_index(proc_queue* q, int index) {
    return q->front + index;
}

void dequeue_proc_at_index(proc_queue* q, int index) {
    int i, prev = 0;

    for (i = 0; i < q->size; i += 1) {
        if (i > index) {
            q->elements[prev] = q->elements[i];
        }

        prev = i;
    }

    q->size--;
    q->rear--;
}

void print_proc_queue(proc_queue* q) {
    int counter;

    PROCESS* proc;

    printf("\tInput queue: [ ");
    for (counter = 0; counter < q->size; counter += 1) {
        proc = peek_queue_at_index(q, iterate_queue_index(q, counter));

        printf("%d ", proc->pid);
    }
    printf("]\n");
}

