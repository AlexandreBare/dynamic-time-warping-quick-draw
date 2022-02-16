#ifndef  BOUNDEDPRIORITYQUEUE_H_DEFINED
#define  BOUNDEDPRIORITYQUEUE_H_DEFINED

#include <stddef.h>
#include <stdbool.h>

/* Opaque structure of a bounded priority queue */
typedef struct bounded_priority_queue_t BoundedPriorityQueue;

/* ------------------------------------------------------------------------- *
 * Create a bounded priority queue.
 *
 * PARAMETERS
 * capacity         Maximum number of element that can be stored in the queue.
 *
 * NOTE
 * The returned structure should be cleaned with `bpqFree` after
 * usage.
 *
 * RETURN
 * bpq              The bounded priority queue.
 * ------------------------------------------------------------------------- */
BoundedPriorityQueue* bpqCreate(size_t capacity);

/* ------------------------------------------------------------------------- *
 * Free the memory allocated for the bounded priority queue.
 *
 * PARAMETERS
 * bpq              The bounded priority queue to free
 * ------------------------------------------------------------------------- */
void bpqFree(BoundedPriorityQueue* bpq);

/* ------------------------------------------------------------------------- *
 * Insert a key/value pair in the priority queue. If the queue is full,
 * the element is not added.
 *
 *
 * PARAMETERS
 * bpq              The bounded priority queue
 * key              A key...
 * value            ... and its associated value
 *
 * RETURN
 * opened           True if the element was successfully added, false if the
 *   			    queue is full
 * ------------------------------------------------------------------------- */
bool bpqInsert(BoundedPriorityQueue* bpq, double key, size_t value);


/* ------------------------------------------------------------------------- *
 * Replace the element with maximum key with a new key/value pair. The queue
 * must contain at least one element (otherwise calling the function results in
 * an undefined behavior).
 *
 * PARAMETERS
 * bpq              The bounded priority queue
 * key              The new key...
 * value            ... and its associated value
 * ------------------------------------------------------------------------- */
void bpqReplaceMaximum(BoundedPriorityQueue* bpq, double key, size_t value);

/* ------------------------------------------------------------------------- *
 * Return the maximum key currently stored in the priority queue. The queue
 * must contain at least one element (otherwise calling the function results in
 * an undefined behavior).
 *
 * PARAMETERS
 * bpq              The bounded priority queue
 *
 * RETURN
 * max              The value of the maximum key.
 * ------------------------------------------------------------------------- */
double bpqMaximumKey(const BoundedPriorityQueue* bpq);

/* ------------------------------------------------------------------------- *
 * Get all the values stored in the queue as an array of unsigned integers.
 * The size of the array is the size of the queue (see function `bpqSize`).
 * The array must be freed after usage.
 *
 * PARAMETERS
 * bpq              The bounded priority queue
 *
 * RETURN
 * array            The array of values.
 * ------------------------------------------------------------------------- */
size_t* bpqGetItems(const BoundedPriorityQueue* bpq);

/* ------------------------------------------------------------------------- *
 * Return the size of the queue (i.e. the number of elements currently stored
 * in the structure).
 *
 * PARAMETERS
 * bpq              The bounded priority queue
 *
 * RETURN
 * size 		    The size of the queue
 * ------------------------------------------------------------------------- */
size_t bpqSize(const BoundedPriorityQueue* bpq);

/* ------------------------------------------------------------------------- *
 * Return the capacity of the queue.
 *
 * PARAMETERS
 * bpq              The bounded priority queue
 *
 * RETURN
 * capacity 	    The capacity of the queue
 * ------------------------------------------------------------------------- */
size_t bpqCapacity(const BoundedPriorityQueue* bpq);
#endif // BOUNDEDPRIORITYQUEUE_H_DEFINED
