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
void init_m(T **a, T **b, unsigned t){
	for(unsigned i=0; i<t; i++){
		a[i] = new T[t];
		b[i] = new T[t];
		for(unsigned j=0; j<t; j++){
			a[i][j] = (T)rand()%10;
			b[i][j] = (T)rand()%10;
		}
	}
}

template<class T>
void init_r(T **a, unsigned t){
	for(unsigned i=0; i<t; i++){
		a[i] = new T[t];
		for(unsigned j=0; j<t; j++){
			a[i][j] = 0;
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

	/*struct timeval ti, tf;
	double ttime;	

	unsigned n = 8;
	unsigned tb[n] = {4,8,16,32,64,128,256,512};

	for(unsigned i=64; i<=2048; i*=2){
		type **a = new type*[i];
		type **b = new type*[i];
		type **r = new type*[i];

		init_m(a,b,i);
		init_r(r,i);

		gettimeofday(&ti, NULL);
			normal_mult(a,b,r,i);
		gettimeofday(&tf, NULL);
		ttime = (tf.tv_sec - ti.tv_sec)*1000 + (tf.tv_usec - ti.tv_usec)/1000;
		printf("%d & %.7f & ", i, ttime/1000);
		delete []r;

		for(unsigned j=0; j<n; j++){
			type **r = new type*[i];
			init_r(r,i);

			gettimeofday(&ti, NULL);
				blocked_mult(a,b,r,i,tb[j]);
			gettimeofday(&tf, NULL);

			ttime = (tf.tv_sec - ti.tv_sec)*1000 + (tf.tv_usec - ti.tv_usec)/1000;
			printf("%.7f & ", ttime/1000);
			delete []r;
		}
		cout << "\n";
		delete []a;		delete []b;		
	}*/
	return 0;
}

// plot "o.dat" using 1:2 w l title "normal", "" using 1:3 w l title "B4", "" using 1:4 w l title "B8", "" using 1:5 w l title "B16", "" using 1:6 w l title "B32", "" using 1:7 w l title "B64", "" using 1:8 w l title "B128", "" using 1:9 w l title "B256", "" using 1:10 w l title "B512";
// plot "o2.dat" using 1:2 w l title "normal", "" using 1:3 w l title "B4", "" using 1:4 w l title "B8", "" using 1:5 w l title "B16", "" using 1:6 w l title "B32", "" using 1:7 w l title "B64", "" using 1:8 w l title "B128", "" using 1:9 w l title "B256", "" using 1:10 w l title "B512";
// plot "o3.dat" using 1:2 w l title "normal", "" using 1:3 w l title "B4", "" using 1:4 w l title "B8", "" using 1:5 w l title "B16", "" using 1:6 w l title "B32", "" using 1:7 w l title "B64", "" using 1:8 w l title "B128", "" using 1:9 w l title "B256", "" using 1:10 w l title "B512";