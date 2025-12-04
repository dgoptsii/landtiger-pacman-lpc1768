#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "queue.h"
// Define the struct type


// Initialize the queue
void initQueue(CircularQueue *q) {
    q->front = 0;
    q->rear = -1;
    q->size = 0;
}

void clearQueue(CircularQueue *q) {
    q->front = 0;
    q->rear = -1;
    q->size = 0;
}

// Check if the queue is empty
bool isEmpty(CircularQueue *q) {
    return q->size == 0;
}

// Check if the queue is full
bool isFull(CircularQueue *q) {
    return q->size == QUEUE_CAPACITY;
}

// Enqueue an element
bool enqueue(CircularQueue *q, QueueElement element) {
    if (isFull(q)) {
        return false; // Queue is full
    }

    q->rear = (q->rear + 1) % QUEUE_CAPACITY;
    q->buffer[q->rear] = element;
    q->size++;
    return true;
}

// Dequeue an element
bool dequeue(CircularQueue *q, QueueElement *element) {
    if (isEmpty(q)) {
        return false; // Queue is empty
    }

    *element = q->buffer[q->front];
    q->front = (q->front + 1) % QUEUE_CAPACITY;
    q->size--;
    return true;
}

// Peek at the front element without removing it
bool peek(CircularQueue *q, QueueElement *element) {
    if (isEmpty(q)) {
        return false; // Queue is empty
    }

    *element = q->buffer[q->front];
    return true;
}