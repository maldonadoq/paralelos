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
void blocked_mult(T **a, T **b, T **r, unsigned t, unsigned bl){
	unsigned i, j, k, i0, j0, k0;
	for(i0=0; i0<t; i0+=bl){
		for(j0=0; j0<t; j0+=bl){
			for(k0=0; k0<t; k0+=bl){
				for(i=i0; i<min(i0+bl,t); i++){
					for(j=j0; j<min(j0+bl,t); j++){
						for(k=k0; k<min(k0+bl,t); k++){
							r[i][j] += a[i][k]*b[k][j];
						}
					}
				}
			}
		}
	}
}

typedef int type;
int main(int argc, char const *argv[]){

	unsigned t = 512, bl = 32;
	type **a = new type*[t];
	type **b = new type*[t];
	type **r = new type*[t];

	init(a,b,r,t);

	struct timeval ti, tf;
	double ttime;

	gettimeofday(&ti, NULL);
	blocked_mult(a,b,r,t,bl);
	gettimeofday(&tf, NULL);
	ttime = (tf.tv_sec - ti.tv_sec)*1000 + (tf.tv_usec - ti.tv_usec)/1000;	

	printf("time blocked_mult %.10f s\n", ttime/1000);

	delete []a;		delete []r;
	return 0;
}