#ifndef _FUNC_H_
#define _FUNC_H_

template<class T>
void print_matrix(T *m, unsigned rows, unsigned cols){
	unsigned i,j;
	for(i=0; i<rows; i++){
		for(j=0; j<cols; j++){
			std::cout << m[(i*cols)+j] << " ";			
		}
		std::cout << "\n";
	}

	std::cout << "\n";
}

template<class T>
void print_matrix(T **a, unsigned rows, unsigned cols){
	for(unsigned i=0; i<rows; i++){
		for( unsigned j=0; j<cols; j++)
			std::cout << a[i][j] << " ";
		std::cout << "\n";
	}
	std::cout << "\n";
}

template<class T>
void print_vector(T *v, unsigned s){
	for(unsigned i=0; i<s; i++)	std::cout << v[i] << " ";
	std::cout << "\n\n";
}

#endif