#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef IntegerMatrix_class
#define IntegerMatrix_class


typedef struct _IntegerMatrix {
    // class methods
    int (*getElement)(struct _IntegerMatrix *self, int i, int j);           
    void (*setElement)(struct _IntegerMatrix *self, int i, int j, int elem);
    void (*print)(struct _IntegerMatrix *self);
    int (*getLines)(struct _IntegerMatrix *self);
    int (*getColumns)(struct _IntegerMatrix *self);
    int (*infNorm)(struct _IntegerMatrix *self);

    
    // attributes
    int ____lines, ____columns;
    int *____matrix_data;    

} IntegerMatrix;

typedef struct  _FileMatrixReturnInteger{
    int lines, columns;
    int *data;
} FileMatrixReturnInteger;



int *___alloc_null_matrix_data_____int___(int lines, int columns) {
    int i;
    int *intmatrix_data = (int *) malloc (sizeof(int) * lines * columns);


    for (i = 0; i < lines * columns; intmatrix_data[i++] = 0);

    return intmatrix_data;

} 

int *___alloc_identity_matrix_____int___(int lines) {
    int i;
    int *intmatrix_data = ___alloc_null_matrix_data_____int___(lines, lines);
    
    for(i = 0; i < lines; i++) {
        intmatrix_data[i * lines + i] = 1;        
    }

    return intmatrix_data;
} 

int *___create_copy_____int___(int *original_data, int lines, int columns) {
    int i;
    int *intmatrix_data = (int *) malloc (sizeof(int) * lines * columns);
    
    for(i = 0; i < lines * columns; i++) {
        intmatrix_data[i] = original_data[i];
    }

    return intmatrix_data;
} 


int ___getlines_____int___(IntegerMatrix *self) {
    return self->____lines;
}

int ___getcolumns_____int___(IntegerMatrix *self) {
    return self->____columns;
}

int ___getelement_____int___(IntegerMatrix *self, int i, int j) {
    return self->____matrix_data[self->getColumns(self) * i + j];
}

void ___setelement_____int___(IntegerMatrix *self, int i, int j, int elem) {
    self->____matrix_data[self->getColumns(self) * i + j] = elem;
}

void ___print_____int___(IntegerMatrix *self) {
    int i, j;
    for(i = 0; i < self->getLines(self); i++) {
        for(j = 0; j < self->getColumns(self); j++) {
            printf("%d ", self->getElement(self, i, j) );
        }
        printf("\n");
    }
}

int ___infinitynorm_____int___(IntegerMatrix *self) {
    int i, j;
    float infinity_norm = abs(self->getElement(self, 0, 0));
    for(i = 0; i < self->getLines(self); i++) {
        for(j = 0; j < self->getColumns(self); j++) {
            infinity_norm = (infinity_norm < abs(self->getElement(self, i, j)) ) ? abs(self->getElement(self, i, j)): infinity_norm;
        }
    }

    return infinity_norm;
}

FileMatrixReturnInteger * ___alloc_matrix_from_file_____int___(char *filename) {
    int i;
    FileMatrixReturnInteger *fdata_return = (FileMatrixReturnInteger *) malloc ( sizeof(FileMatrixReturnInteger)  );
    FILE *fp;
    
    // opening file
    fp = fopen(filename, "r");
    // check if file can be opened
    if(fp == NULL) return NULL;
    
    // reading size
    fscanf(fp, "%d", &fdata_return->lines); fscanf(fp,"%d", &fdata_return->columns); 
    // allocating matrix in memory
    fdata_return->data = (int *) malloc (sizeof(int) * fdata_return->lines * fdata_return->columns);
    //reading matrix
    for(i = 0; i < fdata_return->lines * fdata_return->columns; i++) {
        fscanf(fp, "%d", &fdata_return->data[i]);
    }

    fclose(fp);
    return fdata_return;
} 




IntegerMatrix *new_IntegerMatrix_eye(int lines) {
    IntegerMatrix *F = (IntegerMatrix *) malloc ( sizeof( IntegerMatrix )  );

    F->____lines = lines;
    F->____columns = lines;
    F->____matrix_data = ___alloc_identity_matrix_____int___(lines);
    
    // Assigning methods
    F->getColumns = &___getcolumns_____int___;
    F->getLines = &___getlines_____int___;
    F->getElement = &___getelement_____int___;
    F->print = &___print_____int___;
    F->setElement = &___setelement_____int___;
    F->infNorm = &___infinitynorm_____int___;

    return F;   
}

IntegerMatrix *new_IntegerMatrix_null(int lines, int columns) {
    IntegerMatrix *F = (IntegerMatrix *) malloc ( sizeof( IntegerMatrix )  );


    F->____matrix_data = ___alloc_null_matrix_data_____int___(lines, columns);
    F->____lines = lines;
    F->____columns = columns;

    // Assigning methods
    F->getColumns = &___getcolumns_____int___;
    F->getLines = &___getlines_____int___;
    F->getElement = &___getelement_____int___;
    F->print = &___print_____int___;
    F->setElement = &___setelement_____int___;
    F->infNorm = &___infinitynorm_____int___;

    return F;   
}

IntegerMatrix *new_IntegerMatrix_file(char *filename) {
    IntegerMatrix *F = (IntegerMatrix *) malloc ( sizeof( IntegerMatrix )  );
    FileMatrixReturnInteger *freturn = ___alloc_matrix_from_file_____int___(filename);
    // check if file can be opened
    if(freturn == NULL) return NULL;    
    
    //"unpacking return"
    F->____lines = freturn->lines; F->____columns = freturn->columns;
    F->____matrix_data = freturn->data;

    // Assigning methods
    F->getColumns = &___getcolumns_____int___;
    F->getLines = &___getlines_____int___;
    F->getElement = &___getelement_____int___;
    F->print = &___print_____int___;
    F->setElement = &___setelement_____int___;
    F->infNorm = &___infinitynorm_____int___;


    return F;   
}

IntegerMatrix *new_IntegerMatrix_copy(IntegerMatrix *self) {
    IntegerMatrix *F = (IntegerMatrix *) malloc ( sizeof( IntegerMatrix )  );
    //copying
    F->____matrix_data = ___create_copy_____int___(self->____matrix_data, self->getLines(self), self->getColumns(self) );
    F->____lines = self->getLines(self);
    F->____columns = self->getColumns(self);
    
    // Assigning methods
    F->getColumns = &___getcolumns_____int___;
    F->getLines = &___getlines_____int___;
    F->getElement = &___getelement_____int___;
    F->print = &___print_____int___;
    F->setElement = &___setelement_____int___;
    F->infNorm = &___infinitynorm_____int___;

    return F;
}

IntegerMatrix *new_IntegerMatrix_subtract(IntegerMatrix *matrix_a, IntegerMatrix *matrix_b) {
    int i, j;
    IntegerMatrix *matrix_c = new_IntegerMatrix_copy(matrix_a);

    // subtracting matrix_a from matrix_b
    for(i = 0; i < matrix_c->getLines(matrix_c); i++) {
        for(j = 0; j < matrix_c->getColumns(matrix_c); j++) { 
            matrix_c->setElement( matrix_c, i, j, matrix_c->getElement(matrix_c, i, j) - matrix_b->getElement(matrix_b, i, j) );
        }
    }

    return matrix_c;
}

IntegerMatrix *new_IntegerMatrix_add(IntegerMatrix *matrix_a, IntegerMatrix *matrix_b) {
    int i, j;
    IntegerMatrix *matrix_c = new_IntegerMatrix_copy(matrix_a);

    // subtracting matrix_a from matrix_b
    for(i = 0; i < matrix_c->getLines(matrix_c); i++) {
        for(j = 0; j < matrix_c->getColumns(matrix_c); j++) { 
            matrix_c->setElement( matrix_c, i, j, matrix_c->getElement(matrix_c, i, j) + matrix_b->getElement(matrix_b, i, j) );
        }
    }

    return matrix_c;
}
#endif
