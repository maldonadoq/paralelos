#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

int thc;
long long n;
int flag;
double sum;
pthread_mutex_t mutex;

double serial_pi(long long n){
	double lsum = 0;
	long long i;
	double factor = 1;

	for(i=0; i<n; i++, factor=-factor)
		lsum += factor/((2*i)+1);

	return 4*lsum;
}

void* thread_busy_inside(void *id){
	int mid = (int)id;
	double factor;

	long long i;
	long long myn = n/thc;
	long long first_i = myn*mid;
	long long last_i = first_i+myn;
	
	factor = (first_i%2 == 0)? 1: -1;

	for(i=first_i; i<last_i; i++, factor=-factor){
		while(flag != mid);
		sum += factor/((2*i)+1);
		flag = (flag+1)%thc;
	}

	return NULL;
}

void* thread_busy_outside(void *id){
	int mid = (int)id;
	double factor, lsum=0;

	long long i;
	long long myn = n/thc;
	long long first_i = myn*mid;
	long long last_i = first_i+myn;
	
	factor = (first_i%2 == 0)? 1: -1;

	for(i=first_i; i<last_i; i++, factor=-factor)
		lsum += factor/((2*i)+1);

	while(flag != mid);
	sum += lsum;
	flag = (flag+1)%thc;

	return NULL;
}

void* thread_mutex(void *id){
	int mid = (int)id;
	double factor, lsum=0;

	long long i;
	long long myn = n/thc;
	long long first_i = myn*mid;
	long long last_i = first_i+myn;
	
	factor = (first_i%2 == 0)? 1: -1;

	for(i=first_i; i<last_i; i++, factor=-factor)
		lsum += factor/((2*i)+1);

	pthread_mutex_lock(&mutex);
	sum += lsum;
	pthread_mutex_unlock(&mutex);

	return NULL;
}

int main(int argc, char const *argv[]){	

	int nn = 5;
	int nth = 5;
	//int pin[] = {10000, 100000, 1000000, 10000000, 100000000}
	int vnn[] = {10000, 100000, 1000000, 10000000, 100000000};
	int vnth[] = {4, 8, 16, 32, 64};

	int i,j,k;
	pthread_t *nthread;
	double time;
	struct timeval ti, tf;

	pthread_mutex_init(&mutex, NULL);

	for(i=0; i<nn; i++){
		n = vnn[i];
		gettimeofday(&ti, NULL);
	   	sum = serial_pi(n);
	    gettimeofday(&tf, NULL);
	    time = (tf.tv_sec - ti.tv_sec)*1000 + (tf.tv_usec - ti.tv_usec)/1000;   
		printf("%d & %.8lf\n", n, time/1000);

		/*for(j=0; j<nth; j++){
			thc = vnth[j];
			nthread = malloc(thc*sizeof(pthread_t));
			
			sum = 0;
		    gettimeofday(&ti, NULL);
			for(k=0; k<thc; k++)	pthread_create(&nthread[k], NULL, thread_busy_inside, (void*)k);
		    for(k=0; k<thc; k++)	pthread_join(nthread[k], NULL);
		    sum = 4*sum;
		    gettimeofday(&tf, NULL);
		    time = (tf.tv_sec - ti.tv_sec)*1000 + (tf.tv_usec - ti.tv_usec)/1000;		  
		    printf("%.8lf\t", time/1000);

		    sum = 0;
		    gettimeofday(&ti, NULL);
			for(k=0; k<thc; k++)	pthread_create(&nthread[k], NULL, thread_busy_outside, (void*)k);
		    for(k=0; k<thc; k++)	pthread_join(nthread[k], NULL);
		    sum = 4*sum;
		    gettimeofday(&tf, NULL);		    
		    time = (tf.tv_sec - ti.tv_sec)*1000 + (tf.tv_usec - ti.tv_usec)/1000;
		    printf("%.8lf\t", time/1000);

			sum = 0;
		    gettimeofday(&ti, NULL);
			for(k=0; k<thc; k++)	pthread_create(&nthread[k], NULL, thread_mutex, (void*)k);
		    for(k=0; k<thc; k++)	pthread_join(nthread[k], NULL);
		    sum = 4*sum;
		    gettimeofday(&tf, NULL);
		    time = (tf.tv_sec - ti.tv_sec)*1000 + (tf.tv_usec - ti.tv_usec)/1000;
			printf("%.8lf\n", time/1000);

			free(nthread);
		}*/
	}	

	pthread_mutex_destroy(&mutex);
	return 0;
}