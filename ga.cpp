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
	std::string output_dir = "";
	if(argc > 1) {
		output_dir = std::string(argv[1]);
	} else {
		output_dir.assign("./");
	}

	// set the target state of the CA here
	//GeneticAlgorithm< CA_Genome<base_genome_size> > ga("0001000100010001000");
	//GeneticAlgorithm< CA_Genome<base_genome_size> > ga("0101000011101000011101100110101100000010110011010010111010001001110001000000000010110001100111000110");
	//GeneticAlgorithm< CA_Genome<base_genome_size> > ga("00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");

        //  GeneticAlgorithm< CA_Genome<base_genome_size> > ga("1111111111000000000011111111110000000000111111111100000000001111111111000000000011111111110000000000");
          GeneticAlgorithm< CA_Genome<base_genome_size> > ga("1111111111000000000011111111110000000000111111111100000000001111111111000000000011111111110000000000");

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

