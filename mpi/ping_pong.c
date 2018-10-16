#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    const int limit = 4;
    int my_rank, comm_sz;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); // id
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); // numero de procesos

    int count = 0;
    int tmp_rank = (my_rank+1)%2;
    while(count < limit){
        if(my_rank == count%2) {
            count++;
            MPI_Send(&count, 1, MPI_INT, tmp_rank, 0, MPI_COMM_WORLD);
            printf("%d sent count %d to %d\n", my_rank, count, tmp_rank);
        }
        else{
            MPI_Recv(&count, 1, MPI_INT, tmp_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("%d received count %d from %d\n", my_rank, count, tmp_rank);
        }
    }
    MPI_Finalize();
}