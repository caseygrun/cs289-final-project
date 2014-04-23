/* ****************************************************
 * CS 289 Final Project
 * Casey Grun, Alex Garruss
 * ****************************************************/


// C++ standard libraries
#include <iostream>
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

	virtual std::bitset<genome_size> phenotype() {
		return sequence;
	}

	void mutate() {
		float mutation_frequency = 0.1;
		int mutations = mutation_frequency * genome_size;
		for (int i = 0; i < mutations; ++i)
		{
			int j = rand() * genome_size / RAND_MAX;
			sequence.set(j,rand()%2);
		}
	}

	fitness_T fitness(std::bitset<genome_size> pt) {
		std::bitset<genome_size> target;
		this->_fitness = (fitness_T) hamming<genome_size>(pt, target);
		return this->_fitness;
	}

	Simple_Genome recombine(Simple_Genome partner) {
		std::bitset<genome_size> seq;
		for (int i = 0; i < genome_size/2; ++i)
		{
			seq.set(i,sequence[i]);
		}
		for (int i = genome_size/2; i < genome_size; ++i)
		{
			seq.set(i,partner.sequence[i]);
		}
		return Simple_Genome(seq);
	}

	template <int gs> friend std::ostream& operator<<(std::ostream& os, const Simple_Genome<gs> &i);


	Simple_Genome() {
		for (int i = 0; i < genome_size; ++i)
		{
			sequence.set(i,rand()%2);	
		} 
	}
	Simple_Genome(std::bitset<genome_size> seq) {
		sequence = seq;
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
	CA_Genome(std::bitset<genome_size> seq) : Simple_Genome<genome_size>(seq) { }
	CA_Genome() : Simple_Genome<genome_size>() { }
};

// ----------------------------------------------------------------------------


template <class genome_T>
class GeneticAlgorithm {
protected:
	std::vector<genome_T> pool;
	std::vector<fitness_T> fitness;

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
			pool[i] = genome_T();
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
				pool[i].fitness(pool[i].phenotype());
			}
		}

		// sort the first `generation_size` elements in order of increasing
		// fitness according to the > operator
		std::partial_sort(pool.begin(), pool.begin() + generation_size, pool.end());

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
	std::vector<float> fitnesses() {
		return fitness;
	}

	bool stop() {
		return generation_counter > max_generations;
	}

	void print_candidates() {
		std::vector<genome_T> c = candidates();

		for (int i = 0; i < c.size(); ++i)
		{
			std::cout << c[i];
		}
	}
	
	GeneticAlgorithm(int _ps = 100, int _gs = 50) : pool_size(_ps), max_generation_size(_gs), pool(_ps), fitness(_ps) {
		generation_counter = 0;
		max_lifetime = 10;
		max_generations = 100;
	}
};


