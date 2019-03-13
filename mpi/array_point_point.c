#include <stdio.h>
#include <mpi.h>

int tam = 10;

int main(int argc, char const *argv[]){
	int my_rank, comm_sz;
	int vect[tam], part, sp, i;
	int sum_local, sum_total;

	MPI_Init(NULL,NULL);

	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);	// MPI my process ID
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);	// number of process

	part = tam/(comm_sz-1);

	if(my_rank == 0){
		for(i=0; i<tam; i++)	// lleno el vector
			vect[i] = i+1; 
	
		sp = 0;		
		for(i=1; i<comm_sz; i++){
			MPI_Send(&vect[sp], part, MPI_INT, i, 0, MPI_COMM_WORLD);
			sp = sp + part;
		}

		sum_total = 0;		
		for(i=1; i<comm_sz; i++){
			MPI_Recv(&sum_local, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			sum_total += sum_local;
		}

		printf("Total: %d\n", sum_total);
	}
	else{
		MPI_Recv(vect, part, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		sum_local = 0;
		for(i=0; i<part; i++)
			sum_local += vect[i];
		//printf("%d\n", sum_local);

		MPI_Send(&sum_local, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}

	MPI_Finalize();

	return 0;
}