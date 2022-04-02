
// Setting values
void mat_zero (float * mat, int rows, int cols);
void mat_identity (float * mat, int dim);
void mat_copy (float * source_m, float * target_m, int rows, int cols);

// Create matrices
float * mat_create (int rows, int cols);
float * mat_create_zero (int rows, int cols);
float * mat_create_identity (int dim);
float * mat_create_copy(float * mat, int rows, int cols);

// Elementary Operations
//Rows
void mat_permute_rows (float * mat, int rows, int cols, int row1, int row2);
void mat_combine_rows (float * mat, int rows, int cols, int source_row, int target_row, float coef);
void mat_scale_row (float * mat, int rows, int cols, int target_row, float coef);

//Columns
void mat_permute_cols (float * mat, int rows, int cols, int col1, int col2);
void mat_combine_cols (float * mat, int rows, int cols, int source_col, int target_col, float coef);
void mat_scale_col (float * mat, int rows, int cols, int target_col, float coef);


// Common operations
float * mat_transpose(float * mat, int rows, int cols);
void mat_sum(float * mat1, float * mat2, int rows, int cols);
void mat_prod(float * mat1, float s, int rows, int cols);
float * mat_mul(float * mat1, float * mat2, int rows1, int cols1, int cols2);
int mat_invert (float * mat, int dim); 
float mat_det(float * mat, int dim);
float mat_trace(float * mat, int dim);
float mat_max(float * mat, int  rows, int cols);

// Visualization
void mat_print(float * mat, int rows, int cols);

// User Params
void mat_scan(float * mat, int rows, int cols);
