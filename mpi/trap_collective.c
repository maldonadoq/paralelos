#include <stdio.h>
#include <math.h>
#include <mpi.h>

// read input data
void get_input(int my_rank, int comm_sz, double *a, double *b, int *n){

	if(my_rank == 0){
		printf("Enter a: "); scanf("%lf",a);
		printf("Enter b: "); scanf("%lf",b);
		printf("Enter n: "); scanf("%d",n);		
	}

	// send and receive message to process
	MPI_Bcast(a, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(b, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(n, 1, MPI_INT, 0, MPI_COMM_WORLD);
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

	get_input(my_rank, comm_sz, &a, &b, &n);

	h = (b-a)/n;
	local_n = n/comm_sz; // number of own n

	local_a = a+(my_rank*local_n*h);	// setea el inicio del x en el intervalo
	local_b = local_a + (local_n*h);	// igual el final
	local_int = trapez(local_a, local_b, local_n, h);	// hallas con la operación

	MPI_Reduce(&local_int, &total_int, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	if(my_rank == 0){
		printf("Trapeze Method: %.12lf\n", total_int);
	}

	MPI_Finalize();
	return 0;
}