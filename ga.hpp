/* ****************************************************
 * CS 289 Final Project
 * Casey Grun, Alex Garruss
 * ****************************************************/


// C++ standard libraries
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>

// C standard libraries
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <cstdio>
#include <math.h> 
#include <climits>

// cellular automaton
#include "ca_engine.cpp"


#define fitness_T float
#define lifetime_T long
#define DEATH_FITNESS INT_MIN

// ----------------------------------------------------------------------------

class Genome {
protected:
	// keeps track of the fitness of the genome
	fitness_T _fitness;


public: 

	// keeps track of the number of GA cycles that the genome has lived
	lifetime_T age;

	// assigns the genome the lowest possible fitness
	void kill() {
		_fitness = DEATH_FITNESS;
	}

	inline bool operator< (const Genome& rhs) const {
		return (_fitness < rhs._fitness);
	}
	inline bool operator> (const Genome& rhs) const {
		return rhs < *this; 
	} 

	Genome() {
		age = 0;
	}
};

// ----------------------------------------------------------------------------


template <int genome_size> class Simple_Genome;
template <int genome_size> std::ostream& operator<<(std::ostream& os, const Simple_Genome<genome_size> &g);

template <int genome_size>
class Simple_Genome : public Genome 
{
public:
	std::bitset<genome_size> sequence;
	std::bitset<genome_size> target;

	virtual std::bitset<genome_size> phenotype() {
		return sequence;
	}

	void mutate() {
		float mutation_frequency = 0.01;
		int mutations = mutation_frequency * genome_size;
		for (int i = 0; i < mutations; ++i)
		{
			double jjj = RAND_MAX;
			jjj = rand() / jjj;
			int j = jjj * genome_size;
			
			//std::cout << j << std::endl;
			sequence.set(j,rand()%2);
		}
	}

	fitness_T fitness(std::bitset<genome_size> pt) {
		this->_fitness = (fitness_T) -hamming<genome_size>(pt, this->target);
		return this->_fitness;
	}

	Simple_Genome recombine(Simple_Genome partner) {
		std::bitset<genome_size> seq;

		//int cross_pos = rand() * genome_size / RAND_MAX;
		double jjj = RAND_MAX;
		jjj = rand() / jjj;
		int cross_pos = jjj * genome_size;

		//std::cout << cross_pos << "\n";
		for (int i = 0; i < cross_pos; ++i)
		{
			seq.set(i,sequence[i]);
		}
		for (int i = cross_pos; i < genome_size; ++i)
		{
			seq.set(i,partner.sequence[i]);
		}
		Simple_Genome offspring(seq, target);
		return offspring;
	}

	template <int gs> friend std::ostream& operator<<(std::ostream& os, const Simple_Genome<gs> &i);
	std::ostream& print_genotype(std::ostream& os) {
		return print_bitset_tsv<genome_size>(os, sequence);
	}
	std::ostream& print_phenotype(std::ostream& os) {
		return print_bitset_tsv<genome_size>(os, phenotype());
	}


	Simple_Genome() {
		for (int i = 0; i < genome_size; ++i)
		{
			sequence.set(i,rand()%2);	
		} 
	}
	Simple_Genome(std::string targ) {
		target = std::bitset<genome_size>(targ);
		for (int i = 0; i < genome_size; ++i)
		{
			sequence.set(i,rand()%2);	
		} 
	}
	Simple_Genome(std::bitset<genome_size> seq) {
		sequence = seq;
	}	
	Simple_Genome(std::bitset<genome_size> seq, std::bitset<genome_size> targ) {
		sequence = seq;
		target = targ;
	}	
	Simple_Genome(std::bitset<genome_size> seq, std::string targ) {
		sequence = seq;
		target = std::bitset<genome_size>(targ);
	}	

};

template <int genome_size> 
std::ostream& operator<<(std::ostream& os, const Simple_Genome<genome_size> &g){
    os << "[" << g.sequence << "]" << std::endl;
    return os;
}

// ----------------------------------------------------------------------------


template <int genome_size>
class CA_Genome : public Simple_Genome<genome_size> {
public:
	virtual std::bitset<genome_size> phenotype() {
		return run_ca<genome_size>(this->sequence,/* chatter */false);
	}

	CA_Genome(Simple_Genome<genome_size> g) { 
		this->sequence = g.sequence;
	}
	CA_Genome() : Simple_Genome<genome_size>() { }
	CA_Genome(std::string targ) : Simple_Genome<genome_size>(targ) { }
	CA_Genome(std::bitset<genome_size> seq) : Simple_Genome<genome_size>(seq) { }
	CA_Genome(std::bitset<genome_size> seq, std::bitset<genome_size> targ) : Simple_Genome<genome_size>(seq, targ) { }
	CA_Genome(std::bitset<genome_size> seq, std::string targ) : Simple_Genome<genome_size>(seq, targ) { }
	
};

// ----------------------------------------------------------------------------


template <class genome_T>
class GeneticAlgorithm {
protected:
	std::vector<genome_T> pool;
	std::vector<fitness_T> fitness;
	std::string target;

	// how many genomes should be contained in the population
	int pool_size;

	// the number of genomes that should be preserved in each generation
	int max_generation_size;

	// number of generations elapsed
	int generation_counter;

	// number of generations to run
	int max_generations;

	// maximum lifetime of a genome
	lifetime_T max_lifetime;

public:
	void initialize() {

		for (int i = 0; i < pool_size; ++i)
		{
			if(target.length() > 0) {
				pool[i] = genome_T(target);
			} else {
				pool[i] = genome_T();
			}
		}
	}

	void generation() {

		// number of species that will be allowed to survive
		int generation_size = max_generation_size;

		// mutate and age each genome in the gene pool
		for (int i = 0; i < pool_size; ++i) 
		{
			pool[i].mutate();
			pool[i].age++;
		}

		// evaluate fitness
		for (int i = 0; i < pool_size; ++i)
		{
			if(pool[i].age > max_lifetime) {
				pool[i].kill();
			} else {
				fitness[i] = pool[i].fitness(pool[i].phenotype());
			}
		}

		// sort the first `generation_size` elements in order of decreasing
		// fitness, according to the > operator
		std::partial_sort(pool.begin(), pool.begin() + generation_size, pool.end(), std::greater<genome_T>());

		// allow the top `generation_size` elements to recombine
		for (int i = generation_size; i < pool_size; ++i)
		{
			int male_j   = rand() % generation_size;
			int female_j = rand() % generation_size;

			genome_T male   = pool[male_j];
			genome_T female = pool[female_j];

			pool[i] = static_cast<genome_T>(male.recombine(female));
		}
		generation_counter++;
	};
	std::vector<genome_T> candidates() {
		return pool;
	}
	std::vector<fitness_T> fitnesses() {
		return fitness;
	}

	bool stop() {
		return generation_counter > max_generations;
	}

	void print_fitness_stats() {
		fitness_T mean;
		fitness_T var;
		fitness_T sum = 0;
		fitness_T sum2 = 0;
		fitness_T max = fitness[0];
		fitness_T min = fitness[0];
		for (int i = 0; i < fitness.size(); i++)
		{
			sum += fitness[i];
			sum2 += fitness[i]*fitness[i];
			max = fitness[i] > max ? fitness[i] : max; 
			min = fitness[i] < min ? fitness[i] : min; 
		}
		mean = sum * 1.0 / fitness.size();
		var = sum2 * 1.0 / fitness.size() - mean * mean;
		std::cout << "Mean: " << mean << "\tVar: " << var << "\tMax: " << max << "\tMin: " << min << std::endl;
	}

	void print_candidates() {
		std::vector<genome_T> c = candidates();

		for (int i = 0; i < c.size(); ++i)
		{
			std::cout << c[i];
		}
	}

	void print_genotypes(std::string filename) {
	    std::ofstream of(filename, std::ios::out);
		for (int i = 0; i < pool.size(); ++i)
		{
			pool[i].print_genotype(of); 
			of << std::endl;
		}
	    of.close(); 
	}
	
	void print_phenotypes(std::string filename) {
	    std::ofstream of(filename, std::ios::out);
		for (int i = 0; i < pool.size(); ++i)
		{
			pool[i].print_phenotype(of); 
			of << std::endl;
		}
	    of.close(); 
	}

	void print_fitnesses(std::string filename) {
	    std::ofstream of(filename, std::ios::out);
		for (int i = 0; i < fitness.size(); ++i)
		{
			of << fitness[i] << std::endl; 
		}
	    of.close(); 
	}

	GeneticAlgorithm(int _ps = 100, int _gs = 50) : pool_size(_ps), max_generation_size(_gs), pool(_ps), fitness(_ps) {
		generation_counter = 0;
		max_lifetime = 10;
		max_generations = 100;
	}

	GeneticAlgorithm(std::string targ, int _ps = 100, int _gs = 50) : pool_size(_ps), max_generation_size(_gs), pool(_ps), fitness(_ps), target(targ) {
		generation_counter = 0;
		max_lifetime = 10;
		max_generations = 100;
	}

};


