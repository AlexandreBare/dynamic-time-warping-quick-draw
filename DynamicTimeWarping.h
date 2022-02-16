#ifndef  DYNAMICTIMEWARPING_H
#define  DYNAMICTIMEWARPING_H

#include <stddef.h>
#include "Sketch.h"

/** ------------------------------------------------------------------------ *
 * Compute the dynamic time warping distance between two sketches.
 *
 * PARAMETERS
 * sketch1       The first Sketch object.
 * sketch2       The second Sketch object.
 * maxDistance   A stop-fast maximum distance (infinity for not constraining).
 *
 * RETURN
 * dtw           The DTW score for this pair of sketches or +inf if
 *               the distance is greater than `maxDistance`
 * ------------------------------------------------------------------------- */
double dtw(Sketch sketch1, Sketch sketch2, double maxDistance);

#endif // DYNAMICTIMEWARPING_H
