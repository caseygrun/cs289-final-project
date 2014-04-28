/* ****************************************************
 * CS 289 Final Project
 * Casey Grun, Alex Garruss
 * ****************************************************/

#include "ga.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>

#define base_genome_size 100


int main(int argc, char* argv[]) {
	// seed random number generator
	std::srand(std::time(0));

	// make find output directory
	std::string output_dir;
	if(argc > 0) {
		output_dir = std::string(argv[1]);
	} else {
		output_dir = std::string("./");
	}


	GeneticAlgorithm< CA_Genome<base_genome_size> > ga;
	ga.initialize();

	int generation = 0;
	do {
		// make output filenames
		std::ostringstream genotype_filename; genotype_filename << output_dir <<  "genotypes_" << generation << ".csv";
		std::ostringstream phenotype_filename; phenotype_filename << output_dir <<  "phenotypes_" << generation << ".csv";
		std::ostringstream fitness_filename; fitness_filename << output_dir << "fitnesses_" << generation << ".csv";

		ga.print_genotypes(genotype_filename.str());
		ga.print_phenotypes(phenotype_filename.str());
		ga.print_fitnesses(fitness_filename.str());

		ga.print_fitness_stats();
		std::cout << "------------------------------------------------------------" << std::endl;
		ga.generation();
		generation++;
	} while(!ga.stop());

	std::vector<CA_Genome<base_genome_size> > candidates = ga.candidates();


	ga.print_candidates();

	// for (int i = 0; i < candidates.size(); ++i)
	// {
	// 	std::cout << candidates[i];
	// }

	return 0;
}

