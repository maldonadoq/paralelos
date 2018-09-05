#include <stdio.h>
#include <mpi.h>

int tam = 16;

int add(int *a, unsigned t){
	int partial = 0;
	for(unsigned i=0; i<t; i++){
		partial += a[i];
	}
	
	return partial;
}

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
			vect[i] = i;

		sp = 0;
		for(i=1; i<comm_sz; i++){
			MPI_Send(&vect[sp], part, MPI_INT, i, 0, MPI_COMM_WORLD);
			sp = sp + part;	// salto para empezar en el otro array
		}
	}
	else{			
		MPI_Recv(&vect[0], part, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);			
	}

	sum_local = add(vect, part);
	MPI_Reduce(&sum_local, &sum_total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if(my_rank == 0){
		printf("Total: %d\n", sum_total);
	}
	MPI_Finalize();

	return 0;
}