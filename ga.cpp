/* ****************************************************
 * CS 289 Final Project
 * Casey Grun, Alex Garruss
 * ****************************************************/

#include "ga.hpp"
#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) {
	GeneticAlgorithm< CA_Genome<100> > ga;
	ga.initialize();


	do {
		ga.print_candidates();
		ga.generation();
		std::cout << "------------------------------------------------------------" << std::endl;
	} while(!ga.stop());

	std::vector<CA_Genome<100> > candidates = ga.candidates();


	ga.print_candidates();

	// for (int i = 0; i < candidates.size(); ++i)
	// {
	// 	std::cout << candidates[i];
	// }

	return 0;
}

