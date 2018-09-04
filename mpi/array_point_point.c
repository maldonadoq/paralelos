#include <stdio.h>
#include <mpi.h>

int tam = 100;

int main(int argc, char const *argv[]){
	int my_rank, comm_sz;
	int vect[tam], part, sp, i;

	MPI_Init(NULL,NULL);

	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);	// MPI my process ID
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);	// number of process

	part = tam/(comm_sz-1);

	if(my_rank == 0){
		for(i=0; i<tam; i++)
			vect[i] = i;

		sp = 0;
		for(i=1; i<comm_sz; i++){
			MPI_Send(&vect[sp], part,MPI_INT, i, 0, MPI_COMM_WORLD);
			sp = sp + part;
		}
	}
	else{		
		for(int i=1; i<comm_sz; i++){
			MPI_Recv(&vect, part, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);			
		}		
	}

	MPI_Finalize();

	return 0;
}