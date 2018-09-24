#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

int size_th;
int mrow, mcol;

double *matrix;
double *vector;
double *result;

void create(){
    matrix = malloc(mrow*mcol*sizeof(double));
    vector = malloc(mcol*sizeof(double));
    result = malloc(mrow*sizeof(double));

    int i,j;
    for(i=0; i<mrow; i++){
        for(j=0; j<mcol; j++)
            matrix[(mcol*i)+j] = 4;
    }

    for(j=0; j<mcol; j++)    vector[j] = 3;
}

void print_vector(double *vec, int sz){
    for(int j=0; j<sz; j++)
        printf("%.2lf\t", vec[j]);
    printf("\n\n");
}

void print_matrix(double *mat, int row, int col){
    for(int i=0; i<row; i++){
        for(int j=0; j<col; j++)
            printf("%.2lf\t", mat[(i*col)+j]);
        printf("\n");
    }
    printf("\n");
}

void mat_vec(void *id){
    int mid = (int)id;

    int local_row = mrow/size_th;
    int first_row = mid*local_row;s
    int last_row = (mid+1)*local_row;

    for(int i=first_row; i<last_row; i++){
        result[i] = 0;
        for(int j=0; j<mcol; j++)
            result[i] += matrix[(i*mcol)+j]*vector[j];
    }
}

void mat_vec_serial(){
    for(int i=0; i<mrow; i++){
        result[i] = 0;
        for(int j=0; j<mcol; j++)
            result[i] += matrix[(i*mcol)+j]*vector[j];
    }    
}

int main(int argc, char const *argv[]){
    pthread_t *nthread;

    int thi;
    double time;
    struct timeval ti, tf;

    int nn = 3;
    int vnn[] = {280, 2800, 28000};

    size_th = 28;
    mcol = 400;
    nthread = malloc(size_th*sizeof(pthread_t));
    for(int i=0; i<nn; i++){        
        mrow = vnn[i];        
        
        create();   // create matrix and vector
        //print_matrix(matrix, mrow, mcol);
        //print_vector(vector, mcol);

        gettimeofday(&ti, NULL);

        for(thi=0; thi<size_th; thi++)
            pthread_create(&nthread[thi], NULL, mat_vec, (void*)thi);

        for(thi=0; thi<size_th; thi++)
            pthread_join(nthread[thi], NULL);

        

        gettimeofday(&tf, NULL);
        time = (tf.tv_sec - ti.tv_sec)*1000 + (tf.tv_usec - ti.tv_usec)/1000;   
        printf("%dx%d & %.8lf ",mrow, mcol,time/1000);



        gettimeofday(&ti, NULL);
        mat_vec_serial();        
        gettimeofday(&tf, NULL);
        time = (tf.tv_sec - ti.tv_sec)*1000 + (tf.tv_usec - ti.tv_usec)/1000;   
        printf("& %.8lf\n",time/1000);        
        //print_vector(result, mrow);

        free(matrix);   free(vector);   free(result);
    }

    free(nthread);

    return 0;
}
// plot "mv.dat" using 1:2 w l title "matrix";