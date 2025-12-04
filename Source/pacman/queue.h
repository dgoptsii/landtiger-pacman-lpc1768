#ifndef CIRCULAR_QUEUE_H
#define CIRCULAR_QUEUE_H

#include <stdint.h>
#include <stdbool.h>
#include "./map.h"

#define QUEUE_CAPACITY 1200 // Define the maximum number of elements in the queue, == WIDTH*HEIGHT*2

// Define the struct type
typedef Coords QueueElement;

// Define the circular queue structure
typedef struct {
    QueueElement buffer[QUEUE_CAPACITY];
    int front;
    int rear;
    int size;
} CircularQueue;

// Function declarations
void initQueue(CircularQueue 	*q);
bool isEmpty(CircularQueue *q);
bool isFull(CircularQueue *q);
bool enqueue(CircularQueue *q, QueueElement element);
bool dequeue(CircularQueue *q, QueueElement *element);
bool peek(CircularQueue *q, QueueElement *element);
void clearQueue(CircularQueue *q);

#endif // CIRCULAR_QUEUE_H
