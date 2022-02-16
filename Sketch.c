
#include "Sketch.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "easyppm.h"

// The class names
static const char* const classNames[] = {"tree","zigzag","hamburger","cloud",
                                         "jail", "broom","door",
                                         "the Eiffel Tower","sun","key"};


Dataset* loadDataset(const char* filepath, FILE* out) {

    Dataset *ds = malloc(sizeof(Dataset));
    if(!ds)
        return NULL;

    FILE *fp = fopen(filepath,"r");
    if(!fp)
    {
        free(ds);
        return NULL;
    }

    fscanf(fp,"%zu",&(ds->size));

    if(out != NULL)
        fprintf(out, "Reading %zu sketches from '%s'...\n",ds->size, filepath);

    ds->sketches = malloc(ds->size*sizeof(Sketch));
    if(!ds->sketches)
    {
        free(ds);
        fclose(fp);
        return NULL;
    }

    for (size_t i = 0; i < ds->size; i++) {
        size_t size_ts;
        fscanf(fp,"%d",&(ds->sketches[i].class));
        fscanf(fp,"%zu",&size_ts);

        ds->sketches[i].size = size_ts;
        ds->sketches[i].points = malloc(size_ts*sizeof(Point));
        ds->sketches[i].strokeStarts = malloc(size_ts*sizeof(bool));
        if(!ds->sketches[i].points || !ds->sketches[i].strokeStarts)
        {
            // Need to free everything up to i (included)
            ds->size = i+1;
            freeDataset(ds);
            fclose(fp);
            return NULL;

        }

        for (int j = 0; j < ds->sketches[i].size; j++) {
            int bv;
            fscanf(fp,"%d %d %d", &(ds->sketches[i].points[j].x),
                   &(ds->sketches[i].points[j].y), &bv);
            ds->sketches[i].strokeStarts[j] = bv;
        }
    }

    fclose(fp);

    return ds;
}

void freeDataset(Dataset* dataset) {
    if(!dataset)
        return;

    for (int i = 0; i < dataset->size; i++) {
        free(dataset->sketches[i].points);
        free(dataset->sketches[i].strokeStarts);
    }
    free(dataset->sketches);
    free(dataset);
}

const char *getClassName(int class) {
    return classNames[class];
}

void sketch2ppm(Sketch sk, const char* filename) {

    int nbpoints = sk.size;

    int bbxmax=0, bbymax=0;

    for (int i = 0; i < nbpoints; i++) {
        if (sk.points[i].x > bbxmax)
            bbxmax = sk.points[i].x;
        if (sk.points[i].y > bbymax)
            bbymax = sk.points[i].y;
    }

    PPM ppm = easyppm_create(bbxmax+10, bbymax+10, IMAGETYPE_PPM);
    ppmcolor black = easyppm_rgb(0, 0, 0);
    ppmcolor white = easyppm_rgb(255, 255, 255);
    easyppm_clear(&ppm, white);

    for (int i = 0; i < (nbpoints-1); i++)
        if (!sk.strokeStarts[i+1])
            easyppm_draw_line(&ppm, 5+sk.points[i].x, 5+sk.points[i].y,
                              5+sk.points[i+1].x, 5+sk.points[i+1].y, black);

    easyppm_write(&ppm, filename);
    easyppm_destroy(&ppm);
}
