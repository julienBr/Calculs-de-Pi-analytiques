#include <iostream>
#include <omp.h>
#include <iomanip>
#include <cstdint>
#include <chrono>
#include <cmath>

#define NB_ITERATIONs 10 000 000
#define NB_THREADS 8

using namespace std::chrono;

int main(int argc, char const* argv[]) {

	uint64_t nbIterations = static_cast<uint64_t>(NB_ITERATIONs);
	uint8_t nbThreads    = static_cast<uint8_t>(NB_THREADS);

	if(argc == 3) {
		nbIterations = static_cast<uint64_t>(atoi(argv[1]));
		nbThreads   = static_cast<uint8_t>(atoi(argv[2]));
	}

	Long double pi = 0.L;
	std::cout.precision(40);

	omp_set_dynamic(0);
	omp_set_num_threads(nbThreads);

	// Suite u : Taille doublé par rapport au nombre d'itérations
	Long double* u = new Long double[2 * nbIterations];
	// Initialisation u1/ u2 pour le calcul de Un
	u[1] = 99.L/100.L;
	u[2] = 4801.L/5000.L;

	// Suite v : Même principe que u
	Long double* v = new Long double[2*nbIterations];
	v[1] = 99.L/4780.L;
	v[2] = -11414399.L/11424200.L;

	high_resolution_clock::time_point time = high_resolution_clock::now();

	// Un = (99/50) * Un-1 - Un-2
	for(std::uint64_t i=3; i<2*nbIterations; ++i)
		u[i] = (99.L/50.L) * static_cast<Long double>(U[i-1]) - static_cast<Long double>(u[i-2]);

	// Vn = (99/2390) * Vn-1 - Vn-2
	for()
}