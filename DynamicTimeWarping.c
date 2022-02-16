#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <assert.h>
#include "DynamicTimeWarping.h" 

#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

/* ------------------------------------------------------------------------- *
 * Compute the mean absolute distance between 2 points
 *
 * PARAMETERS
 * a    The first point
 * b    The second point
 *
 * RETURNS
 * double   The mean absolute distance between the 2 points
 * ------------------------------------------------------------------------- */
static double distance(Point a, Point b){
    return (double)(abs(a.x - b.x) + abs(a.y - b.y))/2;
}

double dtw(Sketch sketch1, Sketch sketch2, double maxDistance){
    assert(maxDistance >= 0);
    
    size_t size1 = MAX(sketch1.size, sketch2.size) + 1; // With the acceleration method,
    size_t size2 = MIN(sketch1.size, sketch2.size) + 1; // choosing carefully the size
                                                // can optimise a bit the execution time
    double dtw[size1][size2];
    
    for(size_t i = 1; i < size1; i++){
        dtw[i][0] = DBL_MAX;
    }
    for(size_t j = 1; j < size2; j++){
        dtw[0][j] = DBL_MAX;
    }
    
    dtw[0][0] = 0;
    double min = DBL_MAX;
    for(size_t i = 1; i < size1; i++){
        for(size_t j = 1; j < size2; j++){
            dtw[i][j] = distance(sketch1.points[i], sketch2.points[j])
                      + MIN(MIN(dtw[i - 1][j - 1], dtw[i][j - 1]),
                            dtw[i - 1][j]);
            
            min = MIN(min, dtw[i][j]);
        }
        
        if(i < size1 - 1){
            min += distance(sketch1.points[sketch1.size - 1],
                            sketch2.points[sketch2.size - 1]);
        }
        
        // Acceleration of the dtw distance computation
        if(min >= maxDistance){
            return DBL_MAX;
        }
        min = DBL_MAX;
    }
    
    return dtw[size1 - 1][size2 - 1];
}
