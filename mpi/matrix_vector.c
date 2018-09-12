#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void allocate(double *local_mat, double *local_x, double* local_y, 
   int local_m, int n, int local_n, MPI_Comm comm){

   local_mat = malloc(local_m*n*sizeof(double));
   local_x = malloc(local_n*sizeof(double));
   local_y = malloc(local_m*sizeof(double));
}

void read_matrix(double *local_mat, int m, int local_m, int n, int my_rank, MPI_Comm comm){
   double *mat = NULL;

   if(my_rank == 0){
		mat = malloc(m*n*sizeof(double));
      for(int i=0; i<m; i++){
         for(int j=0; j<n; j++){
            mat[i*n + j] = rand()%10;
         }
      }

      MPI_Scatter(mat, local_m*n, MPI_DOUBLE, local_mat, local_m*n, MPI_DOUBLE, 0, comm);
      free(mat);
   }
   else{
      MPI_Scatter(mat, local_m*n, MPI_DOUBLE, local_mat, local_m*n, MPI_DOUBLE, 0, comm);
   }
}

void read_vector(double *local_vec, int n, int local_n, int my_rank, MPI_Comm comm){
   double *vec = NULL;

   if(my_rank == 0){
      vec = malloc(n*sizeof(double));

      for(int i=0; i<n; i++)  vec[i] = 3;

      MPI_Scatter(vec, local_n, MPI_DOUBLE, local_vec, local_n, MPI_DOUBLE, 0, comm);
   }
   else{
      MPI_Scatter(vec, local_n, MPI_DOUBLE, local_vec, local_n, MPI_DOUBLE, 0, comm);
   }
}

void mat_vect_mult(double *local_mat, double *local_x, double *local_y, int local_m,int n, int local_n, MPI_Comm comm){
	double *x;
	int local_i, j;
	
	x = malloc(n*sizeof(double));
	MPI_Allgather(local_x, local_n, MPI_DOUBLE, x, local_n, MPI_DOUBLE, comm);

	for (local_i = 0; local_i < local_m; local_i++) {
		local_y[local_i] = 0.0;
		for (j = 0; j < n; j++)
			local_y[local_i] += local_mat[local_i*n+j]*x[j];
   }
   free(x);
}

void print_vector(double *local_vec, int n, int local_n, int my_rank, MPI_Comm comm){
    double* vec = NULL;

    if (my_rank == 0) {
        vec = malloc(n*sizeof(double));
        
        MPI_Gather(local_vec, local_n, MPI_DOUBLE, vec, local_n, MPI_DOUBLE, 0, comm);      
        for(int i = 0; i < n; i++)
            printf("%f ", vec[i]);
        printf("\n");
        
        free(vec);
    }
    else{
      MPI_Gather(local_vec, local_n, MPI_DOUBLE, vec, local_n, MPI_DOUBLE, 0, comm);
   }
}

int main(int argc, char const *argv[]){
    double *local_mat;
    double *local_x, *local_y;

    int m, local_m, n, local_n;
    int my_rank, comm_sz;

    MPI_Comm comm;

    MPI_Init(NULL, NULL);

    MPI_Comm_size(comm, &comm_sz);
    MPI_Comm_rank(comm, &my_rank);

    m = 8;
    n = 8;

    MPI_Bcast(&m, 1, MPI_Init, 0, comm);
    MPI_Bcast(&n, 1, MPI_Init, 0, comm);

    local_m = m/comm_sz;
    local_n = n/comm_sz;

    allocate(local_mat, local_x, local_y, local_m, n, local_n, comm);

    read_matrix(local_mat, m, local_m, n, my_rank, comm);
    read_vector(local_x, n, local_n, my_rank, comm);

    mat_vect_mult(local_mat, local_x, local_y, local_m, n, local_n, comm);

    print_vector(local_y, m, local_m, my_rank, comm);

    free(local_mat);
    free(local_y);
    free(local_x);

    MPI_Finalize();

    return 0;
}
