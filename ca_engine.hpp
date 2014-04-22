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


template <int genome_size>
int run_ca (std::bitset<genome_size> x, std::bitset<genome_size> target);

template <int length>
int hamming(std::bitset<length> &a, std::bitset<length> &b);


