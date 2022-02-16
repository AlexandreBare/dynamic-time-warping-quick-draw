#include "Sketch.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "NearestNeighbours.h"



/** ------------------------------------------------------------------------ *
 * Print the help message.
 *
 * PARAMETERS
 * programName   The name of the program
 * ------------------------------------------------------------------------- */
static void printHelp(const char* programName)
{
    fprintf(stderr, "%s %s %s\n", "Usage:", programName, "[-i item_index] "
            "[-k n_neighbours] [-o output_path] train_filepath test_filepath");
}


/** ------------------------------------------------------------------------ *
 * Save the given sketch in folder `output_path`
 *
 * PARAMETERS
 * output_path   The folder in which to save the sketch
 * sketch        The sketch to save
 * distance      The distance with the query
 * ------------------------------------------------------------------------- */
static void saveNeighbour(const char* output_path, Sketch sketch,
                          double distance) {
    char buffer[256];
    sprintf(buffer, "%s/image_%s_%5.2lf.ppm", output_path,
            getClassName(sketch.class), distance);
    sketch2ppm(sketch, buffer);
}

/** ------------------------------------------------------------------------ *
 * Comparison function for `SketchDistance`
 *
 * PARAMETERS
 * sd1           A `SketchDistance`
 * sd2           A `SketchDistance`
 *
 * RETURN
 * comp          0 if both argument are the same, < 0 if the first argument
 *               is less than the second, > 0 otherwise
 * ------------------------------------------------------------------------- */
static int sketchDistanceCompare(const void* sd1, const void* sd2) {
    double diff = (((SketchDistance*)sd1)->distance -
                   ((SketchDistance*)sd2)->distance);
    return diff < 0 ? -1 : (diff > 0 ? 1 : 0);
}


/** ------------------------------------------------------------------------ *
 * Free all arguments, according to its type
 *
 * PARAMETERS
 * trainingset   Something to free
 * testingset    Something to free
 * neighbours    Something to free
 * ------------------------------------------------------------------------- */
static void freeAll(Dataset* trainingset, Dataset* testingset,
                    SketchDistance* neighbours) {
    if(trainingset != NULL)
        freeDataset(trainingset);
    if(testingset != NULL)
        freeDataset(testingset);
    if(neighbours != NULL)
        free(neighbours);
}


/** ------------------------------------------------------------------------ *
 * Search for the k nearest neighbours of a given sketch in a given
 * reference database.
 *
 * SYNOPSIS
 * search [-i item_index] [-k n_neighbours] [-o output_path] train_filepath
 *        test_filepath
 *
 * DESCRIPTION
 * -i item_index      The index of the test sample to use (Default: random).
 * -k n_neighbours    The number of neighbours to compute (Default: 4).
 * -o output_path     The folder in which to save the sketches (Default: ".").
 * train_filepath     The path to the reference database
 * test_filepath      The path to the testing database
 * ------------------------------------------------------------------------- */
int main(int argc, char *argv[]) {
    srand(time(NULL));

    /* ------------------------------ Parsing ------------------------------ */
    if (argc < 3) {
        fprintf(stderr, "Aborting; There must be at least 2 parameters"
                        " (given %d). \n", argc-1);
        printHelp(argv[0]);
        return EXIT_FAILURE;
    }

    size_t index = rand();
    size_t k = 4;
    char* output_path = ".";
    char* train_path = argv[argc-2];
    char* test_path = argv[argc-1];
    size_t argi = 1;

    while(argi < argc - 2) {
        if(strcmp(argv[argi], "-i") == 0) {
            size_t itemIndex;
            if(sscanf (argv[++argi], "%zu", &itemIndex) != 1) {
                fprintf(stderr, "%s\n", "Aborting; item index ('-i') should be"
                                        " an unsigned integer.");
                return EXIT_FAILURE;
            }
            index = itemIndex;
        } else if(strcmp(argv[argi], "-k") == 0) {
            size_t numberOfNeigbours;
            if(sscanf (argv[++argi], "%zu", &numberOfNeigbours) != 1) {
                fprintf(stderr, "%s\n", "Aborting; number of neighbours ('-k')"
                                        " should be an unsigned integer.");
                return EXIT_FAILURE;
            }
            k = numberOfNeigbours;
        } else if(strcmp(argv[argi], "-o") == 0) {
            output_path = argv[++argi];
        } else {
            fprintf(stderr, "%s '%s'\n", "Aborting; unknown parameter",
                    argv[argi]);
            printHelp(argv[0]);
            return EXIT_FAILURE;
        }
        argi++;
    }

    Dataset* trainingset = NULL;
    Dataset* testingset = NULL;
    SketchDistance* neighbours = NULL;

    /* --------------------------- Load sketches --------------------------- */
    trainingset = loadDataset(train_path, stdout);
    if(!trainingset)
    {
        fprintf(stderr, "Aborting; could not load training set (%s)\n",
                train_path);
        freeAll(trainingset, testingset, neighbours);
        return EXIT_FAILURE;
    }
    testingset = loadDataset(test_path, stdout);
    if(!testingset)
    {
        fprintf(stderr, "Aborting; could not load testing set (%s)\n",
                test_path);
        freeAll(trainingset, testingset, neighbours);
        return EXIT_FAILURE;
    }

    index %= testingset->size;  // to make sure we are in the range
    printf("Class of selected sketch %zu is %s (from test set)\n", index,
           getClassName(testingset->sketches[index].class));
    /* ---------------------- Nearest neighbours search --------------------- */
    neighbours = nearestNeighbours(trainingset, testingset->sketches[index], k);
    if(!neighbours)
    {
        fprintf(stderr, "%s\n", "Aborting, could not find the neighbours.");
        freeAll(trainingset, testingset, neighbours);
        return EXIT_FAILURE;
    }

    /* ------------------------ Display query answer ----------------------- */
    qsort(neighbours, k, sizeof(SketchDistance), sketchDistanceCompare);

    printf("Neighbours (from training set): \n");
    for(size_t i = 0; i < k; ++i) {
        printf("%zu) class '%s' with distance %lf\n", i + 1,
               getClassName(neighbours[i].sketch->class),
                            neighbours[i].distance);

        saveNeighbour(output_path,
                      *(neighbours[i].sketch),
                      neighbours[i].distance);
        
    }
  
    sketch2ppm(testingset->sketches[index], "query.ppm");
    freeAll(trainingset, testingset, neighbours);
    return EXIT_SUCCESS;
}
