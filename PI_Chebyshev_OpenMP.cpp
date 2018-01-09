#include <iostream>
#include <omp.h>
#include <iomanip>
#include <cstdint>
#include <chrono>
#include <cmath>

#define NB_ITER 10000000
#define NB_THREADS 8

int main(int argc, char const *argv[])
{

	uint64_t nb_it = static_cast<uint64_t>(NB_ITER);
	uint8_t nb_th = static_cast<uint8_t>(NB_THREADS);

	if (argc == 3){
	 	nb_th = static_cast<uint8_t>(atoi(argv[1]));
	 	nb_it = static_cast<uint64_t>(atoi(argv[2]));
	}
	long double pi = 0.L;
	std::cout.precision(40);
	
	omp_set_dynamic(0);
	omp_set_num_threads(nb_th);
	
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
	
	using namespace std::chrono;
	high_resolution_clock::time_point time = high_resolution_clock::now();

	//calcul, à partir de U3, Un = (99/50) * Un-1 - Un-2
	for(std::uint64_t i = 3 ; i < 2 * nb_it ; ++i){
		U[i] = (99.L/50.L) * static_cast<long double>(U[i-1]) - static_cast<long double>(U[i-2]);
	}

	//calcul, à partir de V3, Vn = (99/2390) * Vn-1 - Vn-2
	for(std::uint64_t i = 3 ; i < 2 * nb_it ; ++i){
		V[i] = (99.L/2390.L) * static_cast<long double>(V[i-1]) - static_cast<long double>(V[i-2]);
	}

	#pragma omp parallel
	//Calcul de Pi = 8*SUM(-1^(n-1)/(10^(2n-1)*(2n-1)))*4U2n-1-V2n-1)
	#pragma omp for
	for (std::uint64_t i = 1; i < nb_it ; ++i) {
		long double p = 8.L*(std::pow(-1,i-1) / static_cast<long double>(std::pow(10,2*i-1) * static_cast<long double>(2*i-1))) * (4.L*static_cast<long double>(U[2*i-1]) - static_cast<long double>(V[2*i-1]));
		pi += p;
	}
	
	#pragma omp barrier //on attend tout le monde
	#pragma omp master //pour le processus principal

	std::cout << "Pi : " << pi << std::endl;
	std::cout << "Effectué en : " << duration_cast<microseconds>(high_resolution_clock::now() - time).count() << std::endl;
	delete [] U;
	delete [] V;

}
