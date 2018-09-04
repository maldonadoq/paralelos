#include <stdio.h>
#include <math.h>
#include <mpi.h>


// read input data
void get_input(int my_rank, int comm_sz, double *a, double *b, int *n){

	if(my_rank == 0){
		printf("Enter a: "); scanf("%lf",a);
		printf("Enter b: "); scanf("%lf",b);
		printf("Enter n: "); scanf("%d",n);

		// send to process number 0
		for(int i=1; i<comm_sz; i++){
			MPI_Send(a, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
			MPI_Send(b, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
			MPI_Send(n, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
		}
	}
	else{
		// receive from all 
		MPI_Recv(a, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(b, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
}

// math function
double func(double x){
	return x*x;
}

// trapezoid method
double trapez(double a, double b, int n, double h){
	double estimate, x;
	estimate = (func(a)+func(b))/2;

	for(int i=1; i<n; i++){
		x = a+(i*h);
		estimate += func(x);
	}

	estimate = estimate*h;
	return estimate;
}

int main(int argc, char const *argv[]){
	int my_rank, comm_sz, n, local_n;
	double a, b ,h ,local_a, local_b;
	double local_int, total_int;

	MPI_Init(NULL,NULL);

	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);	// MPI my process ID
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);	// number of process

	// get data
	get_input(my_rank, comm_sz, &a, &b, &n);

	h = (b-a)/n;
	local_n = n/comm_sz; // number of own n

	local_a = a+(my_rank*local_n*h);
	local_b = local_a + (local_n*h);
	local_int = trapez(local_a, local_b, local_n, h);

	if(my_rank != 0){
		MPI_Send(&local_int, 1,MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	}
	else{
		total_int = local_int;
		for(int i=1; i<comm_sz; i++){
			MPI_Recv(&local_int, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			total_int += local_int;
		}
	}

	if(my_rank == 0){
		printf("Trapeze Method: %.12lf\n", total_int);
	}

	MPI_Finalize();
	return 0;
}