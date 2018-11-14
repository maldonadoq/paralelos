#include <iostream>
#include <unistd.h>
#include <sys/time.h>
#include "../src/print.h"

using namespace std;

template<class T>
void normal_mult(T **a, T **b, T **r, unsigned t){
	unsigned i,j,z;
	for(i=0; i<t; i++){
        for(j=0; j<t; j++){
            for(z=0; z<t; z++)
                r[i][j] += a[i][z]*b[z][j];
        }
    }
}

int main(int argc, char const *argv[]){	

	unsigned t = 1024;
	int **a = new int*[t];
	int **b = new int*[t];
	int **r = new int*[t];

	for(unsigned i=0; i<t; i++){
		a[i] = new int[t];
		b[i] = new int[t];
		r[i] = new int[t];

		for(unsigned j=0; j<t; j++){
			a[i][j] = j;
			b[i][j] = j;
			r[i][j] = 0;
		}
	}		

	struct timeval ti, tf;
	double time;

	gettimeofday(&ti, NULL);
		normal_mult(a,b,r,t);
	gettimeofday(&tf, NULL);
	time = (tf.tv_sec - ti.tv_sec)*1000 + (tf.tv_usec - ti.tv_usec)/1000;
	printf("[%ix%i] cpu time: %.8lf s\n", t, t, time/1000);
	// print_matrix(r, t, t);

	delete []a;
	delete []b;
	delete []r;	
	return 0;
}