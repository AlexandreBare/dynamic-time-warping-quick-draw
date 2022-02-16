#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <assert.h>
#include "NearestNeighbours.h" 
#include "BoundedPriorityQueue.h"
#include "DynamicTimeWarping.h"

SketchDistance* nearestNeighbours(const Dataset* dataset, Sketch query,
                                  size_t k){
    assert(dataset != NULL && k > 0);
    
    BoundedPriorityQueue *bpq = bpqCreate(k);
    double dmax = DBL_MAX;
    double dist[dataset->size];
    
    for(size_t i = 0; i < dataset->size; i++){
        dist[i] = dtw(dataset->sketches[i], query, dmax);
        
        if(bpqSize(bpq) < k){
        //if the priority queue is not yet full
            
            bpqInsert(bpq, dist[i], i);
            dmax = bpqMaximumKey(bpq);
            // the new max distance will be equal to the max distance
            // of the sketches stored by their index in the priority queue
        }else if(dist[i] < bpqMaximumKey(bpq)){
        //else if the the actual dtw distance is lower than the maximum one stored
            
            bpqReplaceMaximum(bpq, dist[i], i);
            dmax = bpqMaximumKey(bpq);
            // the new max distance will be equal to the max distance
            // of the sketches stored by their index in the priority queue
        }
    }
    
    SketchDistance *sketchDist = malloc(k * sizeof(SketchDistance));
    if(sketchDist == NULL){
        printf("Aborting; Not enough memory to create a pointer to a new"
               "SketchDistance structure\n");
        bpqFree(bpq);
        exit(-1);
    }
    
    size_t *indexes = bpqGetItems(bpq);
    //the indexes of the sketches stored in the priority queue
    
    bpqFree(bpq);
    
    for(size_t i = 0; i < k; i++){
        sketchDist[i].sketch = &dataset->sketches[indexes[i]];
        sketchDist[i].distance = dist[indexes[i]];
    }
    
    free(indexes);
    return sketchDist;
}
