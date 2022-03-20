#include <stdio.h>

#include "picoblas.h"

int main(int argc, char ** argv){

    int i;
    float * mat = mat_create_identity(4);

    // Identity
    printf("Matrix:\n");
    mat_print(mat,4,4);

    mat_invert(mat,4);

    printf("Inverse mat:\n");
    mat_print(mat,4,4);


    // Twice identity
    mat_identity(mat,4);
    for (i = 0; i < 4; i++){
        mat_scale_row(mat,4,4,i,2);
    }

    printf("Matrix:\n");
    mat_print(mat,4,4);

    mat_invert(mat,4);

    printf("Inverse mat:\n");
    mat_print(mat,4,4);


    // 10x identity
    mat_identity(mat,4);
    for (i = 0; i < 4; i++){
        mat_scale_row(mat,4,4,i,10);
    }

    printf("Matrix:\n");
    mat_print(mat,4,4);

    mat_invert(mat,4);

    printf("Inverse mat:\n");
    mat_print(mat,4,4);

    // 2Diag
    mat_identity(mat,4);
    mat_permute_rows(mat,4,4,0,3);
    mat_permute_rows(mat,4,4,1,2);

    printf("Matrix:\n");
    mat_print(mat,4,4);

    mat_invert(mat,4);

    printf("Inverse mat:\n");
    mat_print(mat,4,4);

    // Comb 
    mat_identity(mat,4);
    mat_combine_rows(mat,4,4,0,1,1);
    mat_combine_rows(mat,4,4,0,2,2);
    mat_combine_rows(mat,4,4,0,3,3);
    mat_combine_rows(mat,4,4,1,2,1);
    mat_combine_rows(mat,4,4,1,3,2);
    mat_combine_rows(mat,4,4,2,3,1);

    printf("Matrix:\n");
    mat_print(mat,4,4);

    mat_invert(mat,4);

    printf("Inverse mat:\n");
    mat_print(mat,4,4);

}