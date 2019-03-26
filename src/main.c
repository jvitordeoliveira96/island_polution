#include <stdio.h>
#include <stdlib.h>
#include "Island.h"
#include <time.h>
           

int main(int argc, char *argv[]) {
    // check if input file was informed correctly
    if(argc!=3) {
        printf("The input file was not informed correctly.\n");
        printf("Please, try again using: ./assignment1 <inputfile> <tolerance>\n");
        return -1;
    }

    Island *island1 = new_Island(argv[1], argv[2]);
    
    // check if file can be opened
    if(island1 == NULL){
        printf("The input file could not be found or opened.\n");
        return -1;
    } 
    
    // execute simulation
    island1->simulate(island1);
    // freeing used memory
    free(island1);
    
    return 0;
}
