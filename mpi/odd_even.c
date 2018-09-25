#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mpi.h>

int compare(const void* at, const void* bt){
	int a = *((int*)at);
	int b = *((int*)bt);

	if(a<b)	return -1;
	else if(a==b)	return 0;
	else	return 1;
}

void merge_low(int *my_keys, int *recv_keys, int *tmp_keys, int local_n){
	int mk, ri, ti;

	mk = ri = ti = 0;
	while(ti < local_n){
		if(my_keys[mk] <= recv_keys[ri]){
			tmp_keys[ti] = my_keys[mk];
			ti++;
			mk++;
		}
		else{
			tmp_keys[ti] = recv_keys[ri];
			ti++;
			ri++;
		}
	}

	memcpy(my_keys, tmp_keys, local_n*sizeof(int));
}

void merge_high(int *my_keys, int *recv_keys, int *tmp_keys, int local_n){
	int mk, ri, ti;

	mk = ri = ti = local_n-1;
	while(ti >= 0){
		if(my_keys[mk] >= recv_keys[ri]){
			tmp_keys[ti] = my_keys[mk];
			ti--;
			mk--;
		}
		else{
			tmp_keys[ti] = recv_keys[ri];
			ti--;
			ri--;
		}
	}

	memcpy(my_keys, tmp_keys, local_n*sizeof(int));
}

void odd_even_iter(int *local_vec, int *atmp, int *btmp, int local_n,
	int phase, int even, int odd, int my_rank, int comm_sz, MPI_Comm comm){

	MPI_Status status;

	if(phase%2 == 0){
		if(even >= 0){
			MPI_Sendrecv(local_vec, local_n, MPI_INT, even, 0,
				atmp, local_n, MPI_INT, even, 0, comm, &status);
			if(my_rank%2 != 0)
				merge_high(local_vec, atmp, btmp, local_n);
			else
				merge_low(local_vec, atmp, btmp, local_n);
		}
	}
	else{
		if(odd >= 0){
			MPI_Sendrecv(local_vec, local_n, MPI_INT, odd, 0,
				atmp, local_n, MPI_INT, odd, 0, comm, &status);
			if(my_rank%2 != 0)
				merge_low(local_vec, atmp, btmp, local_n);
			else
				merge_high(local_vec, atmp, btmp, local_n);
		}
	}
}

void sort(int *local_vec, int local_n, int my_rank, int comm_sz, MPI_Comm comm){
	int phase;
	int *atmp, *btmp;
	int even, odd;

	atmp = (int*) malloc(local_n*sizeof(int));
	btmp = (int*) malloc(local_n*sizeof(int));

	if(my_rank%2 != 0){
		even = my_rank-1;
		odd = my_rank+1;
		if(odd == comm_sz)
			odd = MPI_PROC_NULL;
	}
	else{
		even = my_rank+1;
		if(even == comm_sz)
			even = MPI_PROC_NULL;
		odd = my_rank-1;
	}

	qsort(local_vec, local_n, sizeof(int), compare);

	for(phase = 0; phase<comm_sz; phase++)
		odd_even_iter(local_vec, atmp, btmp, local_n, phase,
			even, odd, my_rank, comm_sz, comm);

	free(atmp);
	free(btmp);
}

void print(int *local_vec, int local_n, int my_rank, 
	int comm_sz, MPI_Comm comm){

	int *vec;
	int i, n;

	if(my_rank == 0){
		n = comm_sz*local_n;
		vec = (int*)malloc(n*sizeof(int));

		MPI_Gather(local_vec, local_n, MPI_INT, vec, local_n, MPI_INT, 0, comm);
		for(i=0; i<n; i++)
			printf("%d ", vec[i]);
		printf("\n");
	}
	else{
		MPI_Gather(local_vec, local_n, MPI_INT, vec, local_n, MPI_INT, 0, comm);
	}
}

int main(int argc, char const *argv[]){
	int my_rank, comm_sz;
	int *local_vec;
	int n, local_n;
	MPI_Comm comm;

	MPI_Init(NULL, NULL);
	comm = MPI_COMM_WORLD;

	MPI_Comm_size(comm, &comm_sz);
	MPI_Comm_rank(comm, &my_rank);

	MPI_Bcast(&n, 1, MPI_INT, 0, comm);

	n = 1600;
	//n = 65536; // 2^16
	//n = 16777216; // 2^24
	local_n = n/comm_sz;

	local_vec = malloc(local_n*sizeof(int));

	srand(time(NULL));
	for(int i=0; i<local_n; i++)
		local_vec[i] = rand()%(n*10);

	sort(local_vec, local_n, my_rank, comm_sz, comm);
	print(local_vec, local_n, my_rank, comm_sz, comm);

	free(local_vec);
	MPI_Finalize();
	return 0;
}