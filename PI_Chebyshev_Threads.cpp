#include <iostream>
#include <iomanip>
#include <pthread.h>
#include <cstdint>
#include <chrono>
#include <cmath>

#define NB_ITER 10000000
#define NB_THREADS 8

struct dataLoc {
	long double* ptrU;
	long double* ptrV;
	uint64_t begin;
	uint64_t end;
};

void * partialU( void * _data ) {
	struct dataLoc * localData = (struct dataLoc *) _data;

	for(uint64_t i = localData->begin; i < localData->end; i++){
		localData->ptrU[i] = (99.L/50.L) * static_cast<long double>(localData->ptrU[i-1]) - static_cast<long double>(localData->ptrU[i-2]);
	}
	return nullptr;
}

void * partialV( void * _data ) {
	struct dataLoc * localData = (struct dataLoc *) _data;

	for(uint64_t i = localData->begin; i < localData->end; i++){
		localData->ptrV[i] = (99.L/2390.L) * static_cast<long double>(localData->ptrV[i-1]) - static_cast<long double>(localData->ptrV[i-2]);
	}
	return nullptr;
}

void * partialPi( void * _data ) {
	struct dataLoc * localData = (struct dataLoc *) _data;

	long double *pi = new long double;
	*pi = 0.0f;
	
	for (uint64_t i = localData->begin; i < localData->end; i++) {
		*pi += 8*(std::pow(-1,i-1) / (std::pow(10,2*i-1) * (2*i-1)))*(4.L*localData->ptrU[2*i-1]-localData->ptrV[2*i-1]);
	}

	return (pi);
}

int main(int argc, char const *argv[])
{
	uint64_t nb_it = static_cast<uint64_t>(NB_ITER);
	uint8_t nb_th = static_cast<uint8_t>(NB_THREADS);

	if (argc == 3){
	 	nb_th = static_cast<uint8_t>(atoi(argv[1]));
	 	nb_it = static_cast<uint64_t>(atoi(argv[2]));
	}

	pthread_t thtab[nb_th];
	std::cout.precision(30);

	using namespace std::chrono;
	high_resolution_clock::time_point chrono = high_resolution_clock::now();

	//suite U, taille doublé par rapport au nbr d'iteration
	long double *U = new long double[2*nb_it];
	//initialise U1 et U2 pour le calcul de Un
	U[1] = 99.L/100.L;
	U[2] = 4801.L/5000.L;

	//Suite V, taille doublé
	long double *V = new long double[2*nb_it];

	//initialise V1 et V2 pour le calcul de Vn
	V[1] = 99.L/4780.L;
	V[2] = -11414399.L/11424200.L;

	struct dataLoc *dataLoc = new struct dataLoc[nb_th];
	dataLoc[0].begin = static_cast<uint16_t>(3);
	dataLoc[0].end = static_cast<uint16_t>(nb_it*2/nb_th);
	dataLoc[0].ptrU = U;
	dataLoc[0].ptrV = V;
	for (uint8_t th = 1; th < nb_th; ++th)
	{
		dataLoc[th].begin = static_cast<uint16_t>(th*(nb_it*2)/nb_th);
		dataLoc[th].end = static_cast<uint16_t>((th+1)*(nb_it*2)/nb_th);
		dataLoc[th].ptrU = U;
		dataLoc[th].ptrV = V;
	}

	for (uint8_t th=0; th<nb_th; ++th) {		
			pthread_create(&thtab[th], NULL, partialU, (void*) &dataLoc[th]);
	}
	for (uint8_t th=0; th<nb_th; ++th) {
			pthread_join(thtab[th], NULL);
	}

	for (uint8_t th=0; th<nb_th; ++th) {		
			pthread_create(&thtab[th], NULL, partialV, (void*) &dataLoc[th]);
	}
	for (uint8_t th=0; th<nb_th; ++th) {
			pthread_join(thtab[th], NULL);
	}

	for (uint8_t th = 0; th < nb_th; ++th)
	{
		dataLoc[th].begin = th*nb_it/nb_th;
		dataLoc[th].end = (th+1)*nb_it/nb_th;
	}
	
	for (uint8_t th=0; th<nb_th; ++th) {		
			pthread_create(&thtab[th], NULL, partialPi, (void*) &dataLoc[th]);
	}

	void * r;
	long double * res;

	long double pi = 0.0f; 

	for (uint8_t th=0; th<nb_th; ++th) {
			pthread_join(thtab[th], &r);
			res = (long double *) r;
			
			pi += (*res);
	}

	std::uint64_t offset = duration_cast<microseconds>(high_resolution_clock::now()-chrono).count();
	std::cout << "Pi : " << pi << std::endl;
	std::cout << "Effectué en : "<< offset << std::endl;

	delete [] U;
	delete [] V;
	delete [] dataLoc;

}
