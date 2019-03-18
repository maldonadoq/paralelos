#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

int number_max = 10;

void allocate(int **local_mat, int **local_x, int** local_y, 
   int local_m, int n, int local_n, MPI_Comm comm){

   *local_mat = malloc(local_m*n*sizeof(int));
   *local_x = malloc(local_n*sizeof(int));
   *local_y = malloc(local_m*sizeof(int));
}

void read_matrix(int *local_mat, int m, int local_m,
    int n, int my_rank, MPI_Comm comm){
    int *mat = NULL;

    if(my_rank == 0){
        mat = malloc(m*n*sizeof(int));
        srand(time(NULL));
        for(int i=0; i<m; i++){
            for(int j=0; j<n; j++){
                mat[(i*n) + j] = rand()%number_max;
            }
        }

        MPI_Scatter(mat, local_m*n, MPI_INT, local_mat, local_m*n, MPI_INT, 0, comm);
        free(mat);
    }
    else{
        MPI_Scatter(mat, local_m*n, MPI_INT, local_mat, local_m*n, MPI_INT, 0, comm);
    }
}

void read_vector(int *local_vec, int n, int local_n, int my_rank, MPI_Comm comm){
    int *vec = NULL;

    if(my_rank == 0){
        vec = malloc(n*sizeof(int));

        srand(time(NULL));
        for(int i=0; i<n; i++)  vec[i] = rand()%number_max;

        MPI_Scatter(vec, local_n, MPI_INT, local_vec, local_n, MPI_INT, 0, comm);
        free(vec);
    }
    else{
        MPI_Scatter(vec, local_n, MPI_INT, local_vec, local_n, MPI_INT, 0, comm);
    }
}

void mat_vect_mult(int *local_mat, int *local_x, int *local_y,
    int local_m, int n, int local_n, MPI_Comm comm){
    int *x;
    int local_i, j;

    x = malloc(n*sizeof(int));
    MPI_Allgather(local_x, local_n, MPI_INT, x, local_n, MPI_INT, comm);

    for (local_i = 0; local_i < local_m; local_i++) {
        local_y[local_i] = 0.0;
        for (j = 0; j < n; j++)
            local_y[local_i] += local_mat[(local_i*n) + j]*x[j];
    }
    free(x);
}

void print_vector(int *local_vec, int n, int local_n, int my_rank, MPI_Comm comm){
    int* vec = NULL;

    if (my_rank == 0) {
        vec = malloc(n*sizeof(int));
        
        MPI_Gather(local_vec, local_n, MPI_INT, vec, local_n, MPI_INT, 0, comm);
        for(int i = 0; i < n; i++)
            printf("%d ", vec[i]);
        printf("\n");
        
        free(vec);
    }
    else{
      MPI_Gather(local_vec, local_n, MPI_INT, vec, local_n, MPI_INT, 0, comm);
   }
}

int main(int argc, char const *argv[]){
    int *local_mat;
    int *local_x, *local_y;

    int m, local_m, n, local_n;
    int my_rank, comm_sz;

    MPI_Comm comm;

    MPI_Init(NULL, NULL);
    comm = MPI_COMM_WORLD;
    MPI_Comm_size(comm, &comm_sz);
    MPI_Comm_rank(comm, &my_rank);

    n = 100;
    m = 100;    

    MPI_Bcast(&m, 1, MPI_INT, 0, comm);
    MPI_Bcast(&n, 1, MPI_INT, 0, comm);

    local_m = m/comm_sz;
    local_n = n/comm_sz;

    allocate(&local_mat, &local_x, &local_y, local_m, n, local_n, comm);

    read_matrix(local_mat, m, local_m, n, my_rank, comm);
    read_vector(local_x, n, local_n, my_rank, comm);

    mat_vect_mult(local_mat, local_x, local_y, local_m, n, local_n, comm);

    print_vector(local_y, m, local_m, my_rank, comm);

    free(local_mat);
    free(local_x);
    free(local_y);

    MPI_Finalize();

    return 0;
}