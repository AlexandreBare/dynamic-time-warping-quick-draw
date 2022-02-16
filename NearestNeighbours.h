#ifndef  NEARESTNEIGHBOURS_H_DEFINED
#define  NEARESTNEIGHBOURS_H_DEFINED

#include <stddef.h>
#include "Sketch.h"

/**
 * Store a pair of sketch/distance.
 */
typedef struct sketch_distance_t {
	const Sketch* sketch;              // A pointer a sketch
	double distance;                   // The distance to another sketch
} SketchDistance;

/** ------------------------------------------------------------------------ *
 * Given a query, find the k nearest neighbours in the dataset.
 * The array must be freed after usage.
 *
 * PARAMETERS
 * dataset       A dataset of sketches.
 * query         A query sketch.
 * k             The number of neighbours to find
 *
 * RETURN
 * array		 An array of SketchDistance (of size k) containing the nearest
 * 				 sketches and their corresponding distances.
 * ------------------------------------------------------------------------- */
SketchDistance* nearestNeighbours(const Dataset* dataset, Sketch query,
                                  size_t k);

#endif // NEARESTNEIGHBOURS_H_DEFINED
