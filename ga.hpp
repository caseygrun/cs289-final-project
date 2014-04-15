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


template <int length>
int hamming(std::bitset<length> &a, std::bitset<length> &b) {
	std::bitset<length> diff;
	diff |= a;
	diff ^= b;
	int score = 0;
	for (int i = 0; i < length; ++i)
	{
		score += diff[i];
	}
	return score;
}

#define fitness_T float

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


template <int genome_size> class CA_Genome;
template <int genome_size> std::ostream& operator<<(std::ostream& os, const CA_Genome<genome_size> &g);

template <int genome_size>
class CA_Genome : public Genome 
{
protected:
	std::bitset<genome_size> sequence;

public:
	void mutate() {
		float mutation_frequency = 0.1;
		int mutations = mutation_frequency * genome_size;
		for (int i = 0; i < mutations; ++i)
		{
			int j = rand() * genome_size / RAND_MAX;
			sequence.set(j,rand()%2);
		}
	}

	float fitness() {
		std::bitset<genome_size> target;
		return (float) hamming<genome_size>(sequence, target);
	}

	CA_Genome recombine(CA_Genome partner) {
		std::bitset<genome_size> seq;
		for (int i = 0; i < genome_size/2; ++i)
		{
			seq.set(i,sequence[i]);
		}
		for (int i = genome_size/2; i < genome_size; ++i)
		{
			seq.set(i,partner.sequence[i]);
		}
		return CA_Genome(seq);
	}

	template <int gs> friend std::ostream& operator<<(std::ostream& os, const CA_Genome<gs> &i);


	CA_Genome(std::bitset<genome_size> seq) {
		sequence = seq;
	}	
	CA_Genome() {
		for (int i = 0; i < genome_size; ++i)
		{
			sequence.set(i,rand()%2);	
		} 
	}
};

template <int genome_size> 
std::ostream& operator<<(std::ostream& os, const CA_Genome<genome_size> &g){
    os << "[" << g.sequence << "]" << std::endl;
    return os;
}



template <class genome_T>
class GeneticAlgorithm {
protected:
	std::vector<genome_T> pool;
	std::vector<float> fitness;

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
		std::partial_sort(pool.begin(), pool.begin() + generation_size, pool.end());

		for (int i = generation_size; i < pool_size; ++i)
		{
			int male_j   = rand() * generation_size / RAND_MAX;
			int female_j = rand() * generation_size / RAND_MAX;

			genome_T male   = pool[male_j];
			genome_T female = pool[female_j];

			pool[i] = (genome_T) male.recombine(female);
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
		return generation_counter > 10;
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


