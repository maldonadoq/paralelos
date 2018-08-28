#include <iostream>
#include <unistd.h>
#include <sys/time.h>

using namespace std;

template<class T>
void print(T **a, unsigned t){
	for(unsigned i=0; i<t; i++){
		for( unsigned j=0; j<t; j++)
			cout << a[i][j] << " ";
		cout << "\n";
	}
	cout << "\n";
}

template<class T>
void init(T **a, T **b, T **r1, T **r2, unsigned t){
	for(unsigned i=0; i<t; i++){
		a[i] = new T[t];
		b[i] = new T[t];
		r1[i] = new T[t];
		r2[i] = new T[t];
		for(unsigned j=0; j<t; j++){
			a[i][j] = (T)rand()%10;
			b[i][j] = (T)rand()%10;
			r1[i][j] = 0;
			r2[i][j] = 0;
		}
	}
}

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

	unsigned t = 400, bl = 6;
	type **a = new type*[t];
	type **b = new type*[t];
	type **r1 = new type*[t];
	type **r2 = new type*[t];

	init(a,b,r1,r2,t);

	struct timeval ti, tf;
	double ttimep, ttimeq;

	gettimeofday(&ti, NULL);
	blocked_mult(a,b,r2,t,bl);
	gettimeofday(&tf, NULL);
	ttimeq = (tf.tv_sec - ti.tv_sec)*1000 + (tf.tv_usec - ti.tv_usec)/1000;	

	gettimeofday(&ti, NULL);
	normal_mult(a,b,r1,t);
	gettimeofday(&tf, NULL);
	ttimep = (tf.tv_sec - ti.tv_sec)*1000 + (tf.tv_usec - ti.tv_usec)/1000;		

	printf("time normal_mult %.10f s\n", ttimep/1000);
	printf("time blocked_mult %.10f s\n", ttimeq/1000);
		
	// print(r1,t);
	// print(r2,t);

	delete []a;		delete []b;
	delete []r1;	delete []r2;
	return 0;
}
