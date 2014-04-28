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

template <int genome_size> 
std::ostream& print_bitset(std::ostream& os, const std::bitset<genome_size> &seq){
    for (int i = 0; i < genome_size; i++)
    {
        os << (seq[i] ? "#" : "_");
    }
    return os;
}

template <int genome_size> 
std::ostream& print_bitset_tsv(std::ostream& os, const std::bitset<genome_size> &seq){
    for (int i = 0; i < genome_size; i++)
    {
        os << seq[i] << "\t";
    }
    return os;
}

template <int genome_size> 
std::ostream& print_bitset_vector_tsv(std::ostream& os, const std::vector<std::bitset<genome_size> > &seqs) {
    for (typename std::vector<std::bitset<genome_size> >::iterator i = seqs.begin(); i != seqs.end(); ++i)
    {
        print_bitset_tsv(os, *i) << std::endl;
    }
    return os;
}

template <int genome_size> 
void print_bitset_vector_tsv_file(std::string filename, const std::vector<std::bitset<genome_size> > &seqs) {
    std::ofstream of(filename, std::ios::out);
    print_bitset_tsv(of, seqs);
    of.close(); 
}

template <int length>
int hamming(std::bitset<length> &a, std::bitset<length> &b) 
{
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

template <int genome_size>
std::bitset<genome_size> run_ca (std::bitset<genome_size> x, bool chatter=false)
{
    int i;
    int j;
    int n;
    int step_num;

    // n is the columns
    n = genome_size;

    // step_num are the rows
    step_num = 100;

    std::bitset<genome_size> x_old;

    // print initial conditions
    if(chatter) {
        print_bitset<genome_size>(std::cout, x);
        std::cout << "\t->\t";
    }

    // run automaton
    for ( j = 1; j <= step_num; j++ )
    {
        for ( i = 0; i < n + 2; i++ )
        {
            x_old[i] = x[i];
        }
        for ( i = 1; i <= n; i++ )
        {

        // here is rule 110 
        // current pattern  111 110 101 100 011 010 001 000
        // new state for cell   0   1   1   0   1   1   1   0

        if ( ( x_old[i-1] == 0 && x_old[i] == 0 && x_old[i+1] == 1 ) ||
             ( x_old[i-1] == 0 && x_old[i] == 1 && x_old[i+1] == 0 ) ||
             ( x_old[i-1] == 0 && x_old[i] == 1 && x_old[i+1] == 1 ) ||
             ( x_old[i-1] == 1 && x_old[i] == 0 && x_old[i+1] == 1 ) ||
             ( x_old[i-1] == 1 && x_old[i] == 1 && x_old[i+1] == 0 ) )
        {
                x[i] = 1;
        }

        else
            {
                x[i] = 0;
            }
        }
        //
        //  Enforce periodic boundary conditions.
        //
        // uncomment to turn on
        x[0] = x[n];
        x[n+1] = x[1];

    }

    if(chatter) {
        print_bitset<genome_size>(std::cout, x);
        std::cout << std::endl;
    }

    return x;

}


template <int genome_size>
int run_ca_evaluate(std::bitset<genome_size> x, std::bitset<genome_size> target) {
    
    // run CA 
    x = run_ca(x);

    //
    // check edit distance 
    //
    return hamming(x, target);
}