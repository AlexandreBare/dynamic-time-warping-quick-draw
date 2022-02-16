#ifndef SKETCH_H_DEFINED
#define SKETCH_H_DEFINED

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

/* 2D point structure */
typedef struct point_t {
    int x, y;                // Coordinates of the point
} Point;

/* Sketch strucure */
typedef struct sketch_t {
    int class;               // The class of this sketch
    size_t size;             // The number of points of this sketch
    bool* strokeStarts;      // strokeStarts[i] == 1 iff points[i] is the start
                                                         // of a new polyline
    Point* points;           // The array of points of this sketch
} Sketch;

/* Dataset of sketches */
typedef struct dataset_t {
    Sketch* sketches;        // The array of sketches
    size_t size;             // The number of sketches
} Dataset;


/** ------------------------------------------------------------------------ *
 * Load the dataset of sketches.
 *
 * GRAMMAR
 * <dataset> ::= "number of sketches"
 *               <sketches>
 * <sketches> ::= <sketch> | <sketch>
 *                         | <sketches>
 * <sketch> ::= "class"
 *              "number of points"
 *              <points>
 * <points> ::= <point> | <point>
 *                      | <points>
 * <point> ::= "x coordinate" "y coordinate" <start of new strokes>
 * <start of new strokes> ::= 0 | 1
 *
 * PARAMETERS
 * filepath      The path of the dataset
 * out           The stream to output loading information (NULL for silence)
 * ------------------------------------------------------------------------- */
Dataset* loadDataset(const char* filepath, FILE* out);


 /** ------------------------------------------------------------------------ *
 * Free a dataset (sketches included).
 *
 * PARAMETERS
 * dataset       A dataset of sketches.
 * ------------------------------------------------------------------------- */
void freeDataset(Dataset* dataset);


/** ------------------------------------------------------------------------ *
 * Generate a ppm file from a sketch
 *
 * PARAMETERS
 * sk            A sketch
 * filename      The filename in which to save the sketch
 * ------------------------------------------------------------------------- */
void sketch2ppm(Sketch sk, const char* filename);


 /** ------------------------------------------------------------------------ *
 * Provide the name of the class from its index
 *
 * PARAMETERS
 * class         The interger representation of the class
 *
 * RETURN
 * className     The string representation of the class
 * ------------------------------------------------------------------------- */
const char *getClassName(int class);

#endif // SKETCH_H_DEFINED

