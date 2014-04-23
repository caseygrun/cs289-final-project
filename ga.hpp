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

// cellular automaton
#include "ca_engine.cpp"


#define fitness_T float

// ----------------------------------------------------------------------------

class Genome {
protected:
	fitness_T _fitness;


public: 
	// virtual void mutate();
	// virtual Genome recombine();

	// virtual fitness_T fitness();

	inline bool operator< (const Genome& rhs) const {
		return (_fitness < rhs._fitness);
	}
	inline bool operator> (const Genome& rhs) const {
		return rhs < *this; 
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

	// How many genomes should be contained in the population
	int pool_size;

	// The number of genomes that should be preserved in each generation
	int generation_size;

	int generation_counter;

public:
	void initialize() {
		for (int i = 0; i < pool_size; ++i)
		{
			pool[i] = genome_T();
		}
	}
	void generation() {

		// mutate each genome in the gene pool
		for (int i = 0; i < pool_size; ++i) 
		{
			pool[i].mutate();
		}

		// evaluate fitness
		for (int i = 0; i < pool_size; ++i)
		{
			pool[i].fitness(pool[i].phenotype());
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

			// std::cout << male_j << "," << female_j << std::endl;

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
		return generation_counter > 100;
	}


	void print_candidates() {
		std::vector<genome_T> c = candidates();

		for (int i = 0; i < c.size(); ++i)
		{
			std::cout << c[i];
		}
	}
	
	GeneticAlgorithm(int _ps = 100, int _gs = 50) : pool_size(_ps), generation_size(_gs), pool(_ps), fitness(_ps) {
		generation_counter = 0;
	}
};


