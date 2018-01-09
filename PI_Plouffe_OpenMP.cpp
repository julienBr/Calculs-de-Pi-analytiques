#include <omp.h>
#include <iostream>
#include <cstdlib>
#include <chrono>
#include <cmath>

#define NB_ITER 10000000
#define NB_THREADS 8

int main(int argc, char const *argv[]) {
	uint64_t nb_it = static_cast<uint64_t>(NB_ITER);
	uint8_t nb_th = static_cast<uint8_t>(NB_THREADS);

	if (argc == 3){
	 	nb_th = static_cast<uint8_t>(atoi(argv[1]));
	 	nb_it = static_cast<uint64_t>(atoi(argv[2]));
	}

	long double pi = 0.L;
	
	omp_set_dynamic(0);
	omp_set_num_threads(nb_th);

	using namespace std::chrono;
	high_resolution_clock::time_point time = high_resolution_clock::now();
	
	#pragma omp parallel

	#pragma omp for reduction(+:pi)
	for(unsigned int i = 0; i < nb_it; i++) {
		long double k = static_cast<long double>(i);
		pi += (4/(k*8 + 1) - 2/(k*8 + 4) - 1/(8*k + 5) - 1/(k*8 + 6)) / std::pow(16,k);
	}


	#pragma omp barrier
	#pragma omp master

	std::cout << "Pi : " << std::dec << pi << std::endl;
	std::cout << "Effectué en : "<< duration_cast<microseconds>(high_resolution_clock::now() - time).count() << std::endl;
	
	return 0;
}
