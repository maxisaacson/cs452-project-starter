#include "lab.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

// Define the queue structure
struct queue {
    void **data;              // Array to hold the data
    int size;                 // Current number of elements
    int capacity;             // Maximum capacity of the queue
    int front;                // Index of the front element
    int rear;                 // Index of the rear element
    bool shutdown;            // Flag to indicate shutdown
    pthread_mutex_t lock;     // Mutex for synchronization
    pthread_cond_t not_empty; // Condition variable for non-empty
    pthread_cond_t not_full;  // Condition variable for non-full
};

// Initialize a new queue
queue_t queue_init(int capacity) {
    queue_t q = (queue_t)malloc(sizeof(struct queue));
    if (!q) return NULL;

    q->data = (void **)malloc(capacity * sizeof(void *));
    if (!q->data) {
        free(q);
        return NULL;
    }

    q->size = 0;
    q->capacity = capacity;
    q->front = 0;
    q->rear = 0;
    q->shutdown = false;
    pthread_mutex_init(&q->lock, NULL);
    pthread_cond_init(&q->not_empty, NULL);
    pthread_cond_init(&q->not_full, NULL);

    return q;
}

// Frees all memory and related data, signals all waiting threads
void queue_destroy(queue_t q) {
    if (!q) return;

    free(q->data);
    pthread_mutex_destroy(&q->lock);
    pthread_cond_destroy(&q->not_empty);
    pthread_cond_destroy(&q->not_full);
    free(q);
}

// Adds an element to the back of the queue
void enqueue(queue_t q, void *data) {
    pthread_mutex_lock(&q->lock);
    while (q->size == q->capacity && !q->shutdown) {
        pthread_cond_wait(&q->not_full, &q->lock);
    }
    if (q->shutdown) {
        pthread_mutex_unlock(&q->lock);
        return;
    }
    q->data[q->rear] = data;
    q->rear = (q->rear + 1) % q->capacity;
    q->size++;
    pthread_cond_signal(&q->not_empty);
    pthread_mutex_unlock(&q->lock);
}

// Removes the first element in the queue
void *dequeue(queue_t q) {
    pthread_mutex_lock(&q->lock);
    while (q->size == 0 && !q->shutdown) {
        pthread_cond_wait(&q->not_empty, &q->lock);
    }
    if (q->size == 0 && q->shutdown) {
        pthread_mutex_unlock(&q->lock);
        return NULL;
    }
    void *data = q->data[q->front];
    q->front = (q->front + 1) % q->capacity;
    q->size--;
    pthread_cond_signal(&q->not_full);
    pthread_mutex_unlock(&q->lock);
    return data;
}

// Sets the shutdown flag in the queue
void queue_shutdown(queue_t q) {
    pthread_mutex_lock(&q->lock);
    q->shutdown = true;
    pthread_cond_broadcast(&q->not_empty);
    pthread_cond_broadcast(&q->not_full);
    pthread_mutex_unlock(&q->lock);
}

// Returns true if the queue is empty
bool is_empty(queue_t q) {
    pthread_mutex_lock(&q->lock);
    bool empty = (q->size == 0);
    pthread_mutex_unlock(&q->lock);
    return empty;
}

// Returns true if the queue is in shutdown mode
bool is_shutdown(queue_t q) {
    pthread_mutex_lock(&q->lock);
    bool shutdown = q->shutdown;
    pthread_mutex_unlock(&q->lock);
    return shutdown;
}