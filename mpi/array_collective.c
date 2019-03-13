#include <stdio.h>
#include <mpi.h>

int tam = 10;

int add(int *a, unsigned t){
	int partial = 0, i;
	for(i=0; i<t; i++){
		partial += a[i];
	}
	
	return partial;
}

int main(int argc, char const *argv[]){
	int my_rank, comm_sz;
	int vect[tam], part, i;
	int sum_local, sum_total;

	MPI_Init(NULL,NULL);

	MPI_Comm comm = MPI_COMM_WORLD;
	MPI_Comm_rank(comm, &my_rank);	// MPI my process ID
	MPI_Comm_size(comm, &comm_sz);	// number of process

	part = tam/(comm_sz-1);	

	if(my_rank == 0){
		for(i=0; i<tam; i++){	// lleno el vector
			vect[i] = i;
			printf("%d ", vect[i]);
		}
		printf("\n");

		for(i=1; i<comm_sz; i++){
			MPI_Send(&vect[(i-1)*part], part, MPI_INT, i, 0, comm);
		}
	}
	else{			
		MPI_Recv(vect, part, MPI_INT, 0, 0, comm, MPI_STATUS_IGNORE);			
	}

	sum_local = add(vect, part);
	MPI_Reduce(&sum_local, &sum_total, 1, MPI_INT, MPI_SUM, 0, comm);

	if(my_rank == 0){
		printf("Total: %d\n", sum_total-sum_local);
	}
	MPI_Finalize();

	return 0;
}