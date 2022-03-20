#include <stdio.h>

#include "picoblas.h"

int main(){
    float * my_mat = mat_create(3,2);
    printf("Enter the elements of the matrix:");
    mat_scan(my_mat,3,2);
    printf("Your matrix is: \n");
    mat_print(my_mat,3,2);
    printf("The transpose of your matrix is: \n");
    float * t_mat = mat_transpose(my_mat,3,2);
    mat_print(t_mat,2,3);
}
