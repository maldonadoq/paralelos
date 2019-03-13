#include <iostream>
#include <unistd.h>
#include <sys/time.h>

//sudo apt-add-repository ppa:vokoscreen-dev/vokoscreen sudo apt-get update sudo apt-get install vokoscreen

using namespace std;

template<class T>
void print(T *a, unsigned t){
	for(unsigned i=0; i<t; i++)
		cout << a[i] << " ";
	cout << "\n\n";
}

template<class T>
void init(T **a, T *b, T *c1, T *c2, unsigned t){
	for(unsigned i=0; i<t; i++){
		a[i] = new T[t];

		b[i] = (T)rand()%10;
		c1[i] = 0;
		c2[i] = 0;
		for(unsigned j=0; j<t; j++)
			a[i][j] = (T)rand()%20;
	}
}

template<class T>
void func1(T **a, T *b, T *r, unsigned t){
	for(unsigned i=0; i<t; i++){
		for(unsigned j=0; j<t; j++)
			r[i] += a[i][j]*b[j];
	}
}

template<class T>
void func2(T **a, T *b, T *r, unsigned t){
	for(unsigned j=0; j<t; j++){
		for(unsigned i=0; i<t; i++)
			r[i] += a[i][j]*b[j];
	}
}

typedef int type;
int main(int argc, char const *argv[]){
	unsigned t = 1000;
	type **a = new type*[t];
	type *b = new type[t];
	type *c1 = new type[t];
	type *c2 = new type[t];	

	init(a,b,c1,c2,t);

	struct timeval ti, tf;
	double ttimep, ttimeq;

	// first time
	gettimeofday(&ti, NULL);
	func1(a,b,c1,t);	// func1
	gettimeofday(&tf, NULL);
	ttimep = (tf.tv_sec - ti.tv_sec)*1000 + (tf.tv_usec - ti.tv_usec)/1000;

	// second time
	gettimeofday(&ti, NULL);
	func2(a,b,c2,t);	// func2
	gettimeofday(&tf, NULL);
	ttimeq = (tf.tv_sec - ti.tv_sec)*1000 + (tf.tv_usec - ti.tv_usec)/1000;

	// result time
	printf("time 1 %.10f s\n", ttimep/1000);
	printf("time 2 %.10f s\n", ttimeq/1000);

	// result
	// print(c1,t);
	// print(c2,t);

	delete []a;	delete b;
	delete c1;	delete c2;

	return 0;
}