#include <stdlib.h>
#include <stdio.h>

#include "picoblas.h"

// Matrix value setting
void mat_zero (float * mat, int rows, int cols){

    int i;
    for (i = 0; i < rows*cols; i++){
        mat[i] =  0;
    }

}

void mat_identity (float * mat, int dim){

    mat_zero(mat,dim,dim);

    int i;
    for (i = 0; i < dim; i++){
        mat[i*(1+dim)] = 1;
    }

}

void mat_copy (float * source_m, float * target_m, int rows, int cols){

    int i;
    for (i = 0; i < rows*cols; i++){
        target_m[i] =  source_m[i];
    }
}

// Matrix creation (column first)
float * mat_create (int rows, int cols){
    return malloc(sizeof(float)*rows*cols);
}

float * mat_create_zero (int rows, int cols){

    float * mat = mat_create(rows,cols);
    mat_zero(mat,rows,cols);
    return mat;

}

float * mat_create_identity (int dim){

    float * mat = mat_create(dim,dim);
    mat_identity(mat,dim);
    return mat;

}

float * mat_create_copy(float * mat, int rows, int cols){
    
    float * mat_c = mat_create(rows,cols);
    mat_copy(mat,mat_c,rows,cols);
    return mat_c;

}

// Matrix operations (In place)
// Elementary
void mat_permute_rows (float * mat, int rows, int cols, int row1, int row2){

    if (row1 == row2)
        return;
    
    int i;
    float temp;
    for (i = 0; i < cols; i++){
        temp = mat[row1 + i*rows];
        mat[row1 + i*rows] = mat[row2 + i*rows];
        mat[row2 + i*rows] = temp;
    }

}

void mat_permute_cols (float * mat, int rows, int cols, int col1, int col2){

    if (col1 == col2)
        return;
    
    int i;
    float temp;
    for (i = 0; i < rows; i++){
        temp = mat[i + col1*rows];
        mat[i + col1*rows] = mat[i + col2*rows];
        mat[i + col2*rows] = temp;
    }

}

void mat_combine_rows (float * mat, int rows, int cols, int source_row, int target_row, float coef){

    int i;
    for (i = 0; i < cols; i++){
        mat[target_row + i*rows] += mat[source_row + i*rows]*coef;
    }
    
}

void mat_combine_cols (float * mat, int rows, int cols, int source_col, int target_col, float coef){

    int i;
    for (i = 0; i < rows; i++){
        mat[i + target_col*rows] += mat[i + source_col*rows]*coef;
    }
    
}

void mat_scale_row (float * mat, int rows, int cols, int target_row, float coef){

    int i;
    for (i = 0; i < cols; i++){
        mat[target_row + i*rows] *= coef;
    }
    
}

void mat_scale_col (float * mat, int rows, int cols, int target_col, float coef){

    int i;
    for (i = 0; i < rows; i++){
        mat[i + target_col*rows] *= coef;
    }
    
}

// Usual operations
float * mat_transpose(float * mat, int rows, int cols){

    float * t_mat = mat_create(cols,rows);

    int i,j; 
    for (j = 0; j < cols; j++){
        for(i = 0; i < rows; i++){
            t_mat[j+i*cols] = mat[i+j*rows];
        }
    }

    return t_mat;
}

void mat_sum(float * mat1, float * mat2, int rows, int cols){
    int i;
    for(i = 0; i < rows*cols; i++){
        mat1[i] = mat1[i] + mat2[i];
    }
}

void mat_prod(float * mat1, float s, int rows, int cols){
    int i; 
    for (i = 0; i < rows*cols; i++){
        mat1[i] *= s;
    }
}

float * mat_mul(float * mat1, float * mat2, int rows1, int cols1, int cols2){
    int i,j,k;
        // unable to perform multiplication
    float temp;
    float * mat3 = mat_create(rows1,cols2);
    for(i = 0; i < rows1; i++ ) {
       for (j = 0; j < cols2; j ++ ) {
           temp = 0;
            for ( k = 0; k < cols1; k++) {
                temp += mat1[i+k*rows1]*mat2[k + j*cols1];
                mat3[i+j*rows1] = temp;
             } 
        }   
    }
    return mat3;
}

int mat_invert (float * mat, int dim){

    float * inv = mat_create_identity(dim);

    int i,j,max_j;
    float pivot;
    for (i = 0; i < dim - 1; i++){ // Make triangular (equivalent)
        
        // Get max magnitude pivot (for numerical stability)
        max_j = i;
        for (j = i+1; j < dim; j++){
            if (mat[j+i*dim]*mat[j+i*dim] > mat[max_j+i*dim]*mat[max_j+i*dim]){
                    max_j = j;
                }
        }

        if (mat[max_j+i*dim] == 0)
            return -1; // Matrix is non-invertible

        mat_permute_rows(mat,dim,dim,i,max_j);
        mat_permute_rows(inv,dim,dim,i,max_j);

        // Make zeros
        for (j = i+1; j < dim; j++){
            pivot = -mat[j+i*dim]/mat[i*(1+dim)];
            mat_combine_rows(mat,dim,dim,i,j,pivot);
            mat_combine_rows(inv,dim,dim,i,j,pivot);
        } 
    }

    if (mat[(dim*dim-1)] == 0)
        return -1;  // Matrix is non-invertible

    for (i = dim-1; i > 0; i--){ // Make identity (equivalent)
        
        // Normalize pivot
        pivot = mat[i*(dim+1)];
        mat_scale_row(mat,dim,dim,i,1/pivot);
        mat_scale_row(inv,dim,dim,i,1/pivot);

        // Make zeros
        for (j = i-1; j >= 0; j--){
            pivot = -mat[j+i*dim];
            mat_combine_rows(mat,dim,dim,i,j,pivot);
            mat_combine_rows(inv,dim,dim,i,j,pivot);
        } 
    }

    // Normalize last
    pivot = mat[0];
    mat_scale_row(mat,dim,dim,0,1/pivot);
    mat_scale_row(inv,dim,dim,0,1/pivot);

    // Copy to original pointer
    mat_copy(inv,mat,dim,dim);

    // Free auxiliary matrix
    free(inv);

    return 0; // No error
}
// Determinant
float mat_det(float * mat, int dim){
    int i,j,k;
    float ratio;
    float det = 1.0; 
    // Triangularization by Gauss Elimination
    for(i = 0; i < dim; i++){
        if(mat[i+dim*i] == 0.0){
            return -1.0;
        }
        for(j = i + 1; j < dim; j++){
            ratio = mat[j + i*dim]/mat[i + i*dim];
            for(k = 0; k < dim; k++){
                mat[j+k*dim] = mat[j+k*dim] - ratio*mat[i+k*dim];
            }
        }
    }

    for(i = 0; i < dim; i++){
        det = det * mat[i + i*dim];
    }
    return det;
}
// Trace
float mat_trace(float * mat, int dim){
    int i;
    float trace = 0;
    for(i = 0; i < dim; i++){
        trace += mat[i+i*dim];
    }
    return trace;
}

// Maximum entry of the matrix
float mat_max(float * mat, int  rows, int cols){
    int i,j;
    float max = 0.0;
    for(i = 0; i < rows; i++){
        for(j = 0; j < cols; j++){
           if(mat[i+j*rows] > max){
               max = mat[i+j*rows];
           }
        }
    }
    return max;
}

// Visualization
void mat_print(float * mat, int rows, int cols){

    int i,j;
    for (i = 0; i < rows; i++){
        for (j = 0; j < cols; j++){
            printf("%f ", mat[i+rows*j]);
        }
        printf("\n");
    }
    printf("\n");

}

void mat_scan(float * mat, int rows, int cols){

    int i,j;
    for (i = 0; i < rows; i++){
        for(j = 0; j < cols; j++){
            printf("row: %d, column: %d \n",i,j);
            scanf("%f",&mat[i+j*rows]);
        }
    }
}
