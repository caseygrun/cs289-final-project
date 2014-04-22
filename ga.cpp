/* ****************************************************
 * CS 289 Final Project
 * Casey Grun, Alex Garruss
 * ****************************************************/

#include "ga.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

#define base_genome_size 100


int main(int argc, char* argv[]) {
	std::srand(std::time(0));


	GeneticAlgorithm< CA_Genome<base_genome_size> > ga;
	ga.initialize();

	do {
		ga.print_candidates();
		ga.generation();
		std::cout << "------------------------------------------------------------" << std::endl;
	} while(!ga.stop());

	std::vector<CA_Genome<base_genome_size> > candidates = ga.candidates();


	ga.print_candidates();

	// for (int i = 0; i < candidates.size(); ++i)
	// {
	// 	std::cout << candidates[i];
	// }

	return 0;
}

