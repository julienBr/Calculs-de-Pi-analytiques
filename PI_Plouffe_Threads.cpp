#include <iostream>
#include <iomanip>
#include <cstdint>
#include <chrono>
#include <pthread.h>
#include <cmath>
#include <cstring>

#define NB_ITER 10000000
#define NB_THREADS 8

struct dataLoc {
	uint64_t begin;
	uint64_t end;
};

void * partialMean( void * _data ) {
	struct dataLoc * localData = (struct dataLoc *) _data;

	long double *pi = new long double;
	*pi = 0.0f;
	
	for(uint64_t i = localData->begin; i < localData->end; i++) {
		long double k = static_cast<long double>(i);
		*pi += (4.L/(k*8.L + 1.L) - 2.L/(k*8.L + 4.L) - 1.L/(8.L*k + 5.L) - 1.L/(k*8.L + 6.L)) / std::pow(16,k);
	}

	return (pi);
}

int main(int argc, char const *argv[]) {

	uint64_t nb_it = static_cast<uint64_t>(NB_ITER);
	uint8_t nb_th = static_cast<uint8_t>(NB_THREADS);

	if (argc == 3){
	 	nb_th = static_cast<uint8_t>(atoi(argv[1]));
	 	nb_it = static_cast<uint64_t>(atoi(argv[2]));
	}

	pthread_t thtab[nb_th];

	struct dataLoc *dataLoc = new struct dataLoc[nb_th];
	for (uint8_t th = 0; th < nb_th; ++th)
	{
		dataLoc[th].begin = th*nb_it/nb_th;
		dataLoc[th].end = (th+1)*nb_it/nb_th;
	}

	using namespace std::chrono;
	high_resolution_clock::time_point time = high_resolution_clock::now();
	
	for (uint8_t th=0; th<nb_th; ++th) {		
			pthread_create(&thtab[th], NULL, partialMean, (void*) &dataLoc[th]);
	}
	
	void * r;	
	long double * res;

	long double pi = 0.0f; 

	for (uint8_t th=0; th<nb_th; ++th) {
			pthread_join(thtab[th], &r);
			res = (long double *) r;
			
			pi += (*res);
	}
	
	std::uint64_t offset = duration_cast<microseconds>(high_resolution_clock::now() - time).count();
	std::cout.precision(30);
	std::cout << "Pi : " << std::dec << pi << std::endl;
	std::cout << "Effectué en : " << offset << std::endl;
	
	delete[] dataLoc;
	
	return (EXIT_SUCCESS);
}
