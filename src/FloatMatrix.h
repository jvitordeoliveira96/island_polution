#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#ifndef FloatMatrix_class
#define FloatMatrix_class


typedef struct _FloatMatrix {
    // class methods
    float (*getElement)(struct _FloatMatrix *self, int i, int j);
    void (*setElement)(struct _FloatMatrix *self, int i, int j, float elem);
    void (*print)(struct _FloatMatrix *self);
    int (*getLines)(struct _FloatMatrix *self);
    int (*getColumns)(struct _FloatMatrix *self);
    float (*infNorm)(struct _FloatMatrix *self);

    // attributes
    int ____lines, ____columns;
    float *____matrix_data;    

} FloatMatrix;

typedef struct  _FileMatrixReturn{
    int lines, columns;
    float *data;
} FileMatrixReturn;



float *___alloc_null_matrix_data(int lines, int columns) {
    int i;
    float *fmatrix_data = (float *) malloc (sizeof(float) * lines * columns);


    for (i = 0; i < lines * columns; fmatrix_data[i++] = 0);

    return fmatrix_data;

} 

float *___alloc_identity_matrix(int lines) {
    int i;
    float *fmatrix_data = ___alloc_null_matrix_data(lines, lines);
    
    for(i = 0; i < lines; i++) {
        fmatrix_data[i * lines + i] = 1;        
    }

    return fmatrix_data;
} 

float *___create_copy(float *original_data, int lines, int columns) {
    int i;
    float *fmatrix_data = (float *) malloc (sizeof(float) * lines * columns);
    
    for(i = 0; i < lines * columns; i++) {
        fmatrix_data[i] = original_data[i];
    }

    return fmatrix_data;
} 


int ___getlines(FloatMatrix *self) {
    return self->____lines;
}

int ___getcolumns(FloatMatrix *self) {
    return self->____columns;
}

float ___getelement(FloatMatrix *self, int i, int j) {
    return self->____matrix_data[self->getColumns(self) * i + j];
}

void ___setelement(FloatMatrix *self, int i, int j, float elem) {
    self->____matrix_data[self->getColumns(self) * i + j] = elem;
}

void ___print(FloatMatrix *self) {
    int i, j;
    for(i = 0; i < self->getLines(self); i++) {
        for(j = 0; j < self->getColumns(self); j++) {
            printf("%.5f ", self->getElement(self, i, j) );
        }
        printf("\n");
    }
}

float ___infinitynorm(FloatMatrix *self) {
    int i, j;
    float infinity_norm = fabs(self->getElement(self, 0, 0));
    for(i = 0; i < self->getLines(self); i++) {
        for(j = 0; j < self->getColumns(self); j++) {
            infinity_norm = (infinity_norm < fabs(self->getElement(self, i, j)) ) ? fabs(self->getElement(self, i, j)): infinity_norm;
        }
    }

    return infinity_norm;
}


FileMatrixReturn * ___alloc_matrix_from_file(char *filename) {
    int i;
    FileMatrixReturn *fdata_return = (FileMatrixReturn *) malloc ( sizeof(FileMatrixReturn)  );
    FILE *fp;

    // opening file
    fp = fopen(filename, "r");
    // check if file can be opened
    if(fp == NULL) return NULL;

    // reading size
    fscanf(fp, "%d", &fdata_return->lines); fscanf(fp,"%d", &fdata_return->columns); 
    // allocating matrix in memory
    fdata_return->data = (float *) malloc (sizeof(float) * fdata_return->lines * fdata_return->columns);
    //reading matrix
    for(i = 0; i < fdata_return->lines * fdata_return->columns; i++) {
        fscanf(fp, "%f", &fdata_return->data[i]);
    }

    fclose(fp);
    return fdata_return;
} 




FloatMatrix *new_FloatMatrix_eye(int lines) {
    FloatMatrix *F = (FloatMatrix *) malloc ( sizeof( FloatMatrix )  );

    F->____lines = lines;
    F->____columns = lines;
    F->____matrix_data = ___alloc_identity_matrix(lines);
    
    // Assigning methods
    F->getColumns = &___getcolumns;
    F->getLines = &___getlines;
    F->getElement = &___getelement;
    F->print = &___print;
    F->setElement = &___setelement;
    F->infNorm = &___infinitynorm;


    return F;   
}

FloatMatrix *new_FloatMatrix_null(int lines, int columns) {
    FloatMatrix *F = (FloatMatrix *) malloc ( sizeof( FloatMatrix )  );


    F->____matrix_data = ___alloc_null_matrix_data(lines, columns);
    F->____lines = lines;
    F->____columns = columns;

    // Assigning methods
    F->getColumns = &___getcolumns;
    F->getLines = &___getlines;
    F->getElement = &___getelement;
    F->print = &___print;
    F->setElement = &___setelement;  
    F->infNorm = &___infinitynorm;  

    return F;   
}

FloatMatrix *new_FloatMatrix_file(char *filename) {
    FloatMatrix *F = (FloatMatrix *) malloc ( sizeof( FloatMatrix )  );
    FileMatrixReturn *freturn = ___alloc_matrix_from_file(filename);
    // check if file can be opened
    if(freturn == NULL) return NULL;
    
    //"unpacking return"
    F->____lines = freturn->lines; F->____columns = freturn->columns;
    F->____matrix_data = freturn->data;

    // Assigning methods
    F->getColumns = &___getcolumns;
    F->getLines = &___getlines;
    F->getElement = &___getelement;
    F->print = &___print;
    F->setElement = &___setelement;
    F->infNorm = &___infinitynorm;


    return F;   
}

FloatMatrix *new_FloatMatrix_copy(FloatMatrix *self) {
    FloatMatrix *F = (FloatMatrix *) malloc ( sizeof( FloatMatrix )  );
    //copying
    F->____matrix_data = ___create_copy(self->____matrix_data, self->getLines(self), self->getColumns(self) );
    F->____lines = self->getLines(self);
    F->____columns = self->getColumns(self);
    
    // Assigning methods
    F->getColumns = &___getcolumns;
    F->getLines = &___getlines;
    F->getElement = &___getelement;
    F->print = &___print;
    F->setElement = &___setelement;
    F->infNorm = &___infinitynorm;

    return F;
}

FloatMatrix *new_FloatMatrix_subtract(FloatMatrix *matrix_a, FloatMatrix *matrix_b) {
    int i, j;
    FloatMatrix *matrix_c = new_FloatMatrix_copy(matrix_a);

    // subtracting matrix_a from matrix_b
    for(i = 0; i < matrix_c->getLines(matrix_c); i++) {
        for(j = 0; j < matrix_c->getColumns(matrix_c); j++) { 
            matrix_c->setElement( matrix_c, i, j, matrix_c->getElement(matrix_c, i, j) - matrix_b->getElement(matrix_b, i, j) );
        }
    }

    return matrix_c;
}

FloatMatrix *new_FloatMatrix_add(FloatMatrix *matrix_a, FloatMatrix *matrix_b) {
    int i, j;
    FloatMatrix *matrix_c = new_FloatMatrix_copy(matrix_a);

    // subtracting matrix_a from matrix_b
    for(i = 0; i < matrix_c->getLines(matrix_c); i++) {
        for(j = 0; j < matrix_c->getColumns(matrix_c); j++) { 
            matrix_c->setElement( matrix_c, i, j, matrix_c->getElement(matrix_c, i, j) + matrix_b->getElement(matrix_b, i, j) );
        }
    }

    return matrix_c;
}

#endif
