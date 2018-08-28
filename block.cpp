#include <iostream>
#include <unistd.h>
#include <sys/time.h>

using namespace std;

template<class T>
void init(T **a, T **b, T **r, unsigned t){
	for(unsigned i=0; i<t; i++){
		a[i] = new T[t];
		b[i] = new T[t];
		r[i] = new T[t];
		for(unsigned j=0; j<t; j++){
			a[i][j] = (T)rand()%10;
			b[i][j] = (T)rand()%10;
			r[i][j] = 0;
		}
	}
}

template<class T>
void block(T **a, T **b, T **r, unsigned t, unsigned bl){
	unsigned i,j,k,jj,kk;
	for (k = 0; k < t; k += bl)
		for (j = 0; j < t; j += bl)
			for (i = 0; i < t; ++i)
				for (jj = j; jj < min(j + bl, t); ++jj)
					for (kk = k; kk < min(k + bl, t); ++kk)
						r[i][jj] += a[i][kk] * b[kk][jj];
}

typedef int type;
int main(int argc, char const *argv[]){

	unsigned t = 400, bl = 6;
	type **a = new type*[t];
	type **b = new type*[t];
	type **r = new type*[t];

	init(a,b,r,t);

	struct timeval ti, tf;
	double ttime;

	gettimeofday(&ti, NULL);
	block(a,b,r,t,bl);
	gettimeofday(&tf, NULL);
	ttime = (tf.tv_sec - ti.tv_sec)*1000 + (tf.tv_usec - ti.tv_usec)/1000;	

	printf("time block %.10f s\n", ttime/1000);

	delete []a;		delete []r;
	return 0;
}							