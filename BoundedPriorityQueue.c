#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "BoundedPriorityQueue.h" 


typedef struct node_t{
	double key;
	size_t value;
}Node;

struct bounded_priority_queue_t{
	Node *nodes;
	size_t size;
	size_t capacity;
};



BoundedPriorityQueue* bpqCreate(size_t capacity){
    
	BoundedPriorityQueue *bpq = malloc(sizeof(BoundedPriorityQueue));
	if(bpq == NULL){
        printf("Aborting; Not enough memory to create a bounded priority queue\n");
		exit(-1);
	}
	bpq->nodes = malloc(capacity * sizeof(Node));
	if(bpq->nodes == NULL){
        printf("Aborting; Not enough memory to create %zu nodes in the bounded "
               "priority queue\n", capacity);
		free(bpq);
		exit(-1);
	}
	bpq->capacity = capacity;
	bpq->size = 0;
	return bpq;
}

void bpqFree(BoundedPriorityQueue* bpq){
    assert(bpq != NULL);
    
	free(bpq->nodes);
	free(bpq);
}

/* ------------------------------------------------------------------------- *
 * Increase the key value of the node that is inserted until it reaches the
 * value given in argument, rearranging the parent relations if needed
 *
 * PARAMETERS
 * bpq              The bounded priority queue
 * i                The index of the added key
 * key 				The new key
 * ------------------------------------------------------------------------- */
static void bpqIncreaseKey(BoundedPriorityQueue* bpq, size_t i, double key){
    assert(bpq != NULL);
    
	if (key < bpq->nodes[i].key)
		exit(-1);

	bpq->nodes[i].key = key;
	size_t parent = (i - 1) / 2;
	while(i > 0 && bpq->nodes[parent].key < bpq->nodes[i].key){
		Node tmp = bpq->nodes[i];
		bpq->nodes[i] = bpq->nodes[parent];
		bpq->nodes[parent] = tmp;
		parent = (i - 1) / 2;
		i = parent;
	}
}

bool bpqInsert(BoundedPriorityQueue* bpq, double key, size_t value){
    assert(bpq != NULL);
    
    size_t size = bpqSize(bpq);
	if(size == bpqCapacity(bpq)){
		return false;
	} else {
		bpq->nodes[size].value = value;
		bpq->nodes[size].key = 0;
		bpqIncreaseKey(bpq, size, key);
		bpq->size++;
		return true;
	}
}

/* ------------------------------------------------------------------------- *
 * Rebuild the bounded priority queue when the maximum key changes
 * by rearranging the parent - left - right relations in the keys array
 *
 * PARAMETERS
 * bpq              The bounded priority queue
 * i                The index of the largest key
 * ------------------------------------------------------------------------- */
static void bpqRebuild(BoundedPriorityQueue* bpq, size_t i){
    assert(bpq != NULL);
    
    size_t size = bpqSize(bpq);
	size_t largest = 0;
	size_t left = 2 * i;
	size_t right = 2 * i + 1;
	if(left <= size && bpq->nodes[left].key > bpq->nodes[i].key)
		largest = left;
	else
		largest = i;
    
	if(right <= size && bpq->nodes[right].key > bpq->nodes[largest].key)
		largest = right;
	if(largest != i){
		Node tmp = bpq->nodes[i];
		bpq->nodes[i] = bpq->nodes[largest];
		bpq->nodes[largest] = tmp;
		bpqRebuild(bpq, largest);
	}
}

void bpqReplaceMaximum(BoundedPriorityQueue* bpq, double key, size_t value){
    assert(bpq != NULL);

	if(bpqSize(bpq) < 1){
		printf("Error in bpqReplaceMaximum() : the queue must contain at least one element\n");
        bpqFree(bpq);
		exit(-1);
	}
	Node new;
	new.key = key;
	new.value = value;
	bpq->nodes[0] = new;
	bpqRebuild(bpq, 0);
}

double bpqMaximumKey(const BoundedPriorityQueue* bpq){
    assert(bpq != NULL);

	return bpq->nodes[0].key;
}

size_t* bpqGetItems(const BoundedPriorityQueue* bpq){
    assert(bpq != NULL);
    
    size_t size = bpqSize(bpq);
    if(size == 0)
        return NULL;
    
	size_t *items = malloc(size * sizeof(size_t));
    if(items == NULL){
        printf("Aborting; Not enough memory to create an array with the"
               "%zu elements of the bounded priority queue\n", size);
		exit(-1);
    }
    
	for(size_t i = 0; i < size ; i++){
		items[i] = bpq->nodes[i].value;
	}
	return items;
}

size_t bpqSize(const BoundedPriorityQueue* bpq){
    assert(bpq != NULL);
    
	return bpq->size;
}

size_t bpqCapacity(const BoundedPriorityQueue* bpq){
    assert(bpq != NULL);

	return bpq->capacity;
}
