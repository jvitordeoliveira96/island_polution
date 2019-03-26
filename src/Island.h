#include "IntegerMatrix.h"
#include "FloatMatrix.h"

#ifndef WATER 
#define WATER 0
#endif

#ifndef GROUND 
#define GROUND 1
#endif

#ifndef DIESEL
#define DIESEL 2
#endif

#ifndef ITERMAX
#define ITERMAX 1200
#endif


#ifndef Island_class
#define Island_class


typedef struct _Island {
    // class methods
    void (*print)(struct _Island *self);
    int (*getLines)(struct _Island *self);
    int (*getColumns)(struct _Island *self);
    void (*simulate)(struct _Island *self);
    float (*getTolerance)(struct _Island *self);

    // attributes
    FloatMatrix *poluents;
    IntegerMatrix *cell_type;
    int ____lines, ____columns;
    float ____tolerance;

} Island;

int ___getlines_____isl___(Island *self) {
    return self->____lines;
}

int ___getcolumns_____isl___(Island *self) {
    return self->____columns;
}

float ___gettolerance_____isl___(Island *self) {
    return self->____tolerance;
}

void ___print_____isl___(Island *self) {
    printf("\n*------------- ISLAND of size (%d,%d) -------------*\n", self->getLines(self), self->getColumns(self) );
    printf("The type of each cell is:\n");
    self->cell_type->print(self->cell_type);
    printf("The present state of polution is:\n");
    self->poluents->print(self->poluents);
}

void diffuseIsland(Island *self, int i, int j) {
    float neighboor_up = (i > 0 ) ? self->poluents->getElement( self->poluents, i - 1, j ): 0.;
    float neighboor_down = (i < self->getLines(self) - 1) ? self->poluents->getElement( self->poluents, i + 1, j ): 0.;
    float neighboor_left = (j > 0 ) ? self->poluents->getElement( self->poluents, i, j - 1 ): 0.;
    float neighboor_right = (j < self->getLines(self) - 1) ? self->poluents->getElement( self->poluents, i, j + 1 ): 0.;

    float mean = (neighboor_up + neighboor_down + neighboor_left + neighboor_right)/4.0;

    self->poluents->setElement(self->poluents, i, j, mean);
}

void ___simulate_____isl___(Island *self) {
    int iter = 0, i, j;
    float error = 1;
    FloatMatrix *aux_matrix;
    printf("\n\n--|# of iters = %d|--\n", iter);
    self->print(self);


    do  {
        aux_matrix = new_FloatMatrix_copy(self->poluents);
        for(i = 0; i < self->getLines(self); i++) {
            for(j = 0; j < self->getColumns(self); j++) {
                if( self->cell_type->getElement(self->cell_type, i, j) == GROUND ) diffuseIsland(self, i, j);
            }
        }
        aux_matrix = new_FloatMatrix_subtract(self->poluents, aux_matrix);
        error = aux_matrix->infNorm(aux_matrix) / self->poluents->infNorm(self->poluents);
        free(aux_matrix);
        iter++;

        // showing iterations results
        printf("\n\n--|# of iters = %d|--|relative_err = %.6f|--|tol = %.6f|--\n", iter, error, self->getTolerance(self));
        self->print(self);

    } while (iter < ITERMAX && error > self->getTolerance(self) );
}



Island *new_Island(char *island_file_name, char *tol_str) {
    int i, j;
    float tolerance;
    Island *island = (Island *) malloc ( sizeof( Island )  );
    IntegerMatrix *island_cell_type = new_IntegerMatrix_file(island_file_name);
    
    // check if file can be opened
    if(island_cell_type == NULL) return NULL;
    // creating polution floating point matrix
    FloatMatrix *island_poluents = new_FloatMatrix_null(island_cell_type->getLines(island_cell_type), island_cell_type->getColumns(island_cell_type) );

    // filling poluents matrix
    for(i = 0; i < island_cell_type->getLines(island_cell_type); i++) {
        for(j = 0; j < island_cell_type->getColumns(island_cell_type); j++) {
            float poluent = ( island_cell_type->getElement(island_cell_type, i, j) == DIESEL ) ? 1. : 0;
            island_poluents->setElement(island_poluents, i, j, poluent);  
        }
    }

    // assigning into island object matrices and shape
    island->cell_type = island_cell_type;
    island->poluents = island_poluents;
    island->____lines = island->cell_type->getLines(island->cell_type);
    island->____columns = island->cell_type->getColumns(island->cell_type);
    sscanf(tol_str, "%f", &tolerance);
    island->____tolerance = fabs(tolerance);

    // Assigning methods
    island->getLines = &___getlines_____isl___;
    island->getColumns = &___getcolumns_____isl___;
    island->print = &___print_____isl___;
    island->simulate = &___simulate_____isl___;
    island->getTolerance = &___gettolerance_____isl___;
    
    return island;
}


#endif
