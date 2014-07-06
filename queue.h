typedef struct proc_queue {
    int capacity;
    int size;
    int front;
    int rear;
    PROCESS *elements;
} proc_queue;

proc_queue* create_proc_queue(int length) {
    proc_queue *q;

    q = malloc(sizeof(proc_queue));

    q->elements = (PROCESS *) malloc(sizeof(PROCESS) * length);
    q->size     = 0;
    q->capacity = length;
    q->front    = 0;
    q->rear     = -1;

    return q;
}

void enqueue_proc(proc_queue* q, PROCESS* proc) {
    if (q->size == q->capacity) {
        printf("ERROR: queue is full to capacity!\n");

        exit(2);
    }

    q->size++;
    q->rear += 1;

    if (q->rear == q->capacity) {
        q->rear = 0;
    }

    q->elements[q->rear] = *proc;
}

int queue_has_next(proc_queue* q) {
    return q->size == 0 ? 0 : 1;
}

PROCESS* peek_queue(proc_queue* q) {
    if (!queue_has_next(q)) {
        printf("ERROR: queue is empty, can't get the next element\n");
        exit(2);
    }

    return &q->elements[q->front];
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
