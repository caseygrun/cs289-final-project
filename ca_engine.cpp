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
int run_ca (std::bitset<genome_size> x, std::bitset<genome_size> target)
{
  int i;
  int j;
  int n;
  int step_num;

  //  timestamp ( );

  // n is the columns
  n = genome_size;

  // step_num are the rows
  step_num = 100;

  std::bitset<genome_size> x_old;

  std::cout << x << "\t";

  // for ( i = 0; i <= n + 1; i++ )
  // {
  //   x[i] = 0;
  // }

  // //  initial condition
  // x[n-1] = 1;

  for ( j = 1; j <= step_num; j++ )
  {
    for ( i = 0; i < n + 2; i++ )
    {
      x_old[i] = x[i];
    }
    for ( i = 1; i <= n; i++ )
    {

    // here is rule 110 
    // current pattern	111	110	101	100	011	010	001	000
    // new state for cell	0	1	1	0	1	1	1	0

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
  //    x[0] = x[n];
  //    x[n+1] = x[1];

  }

  std::cout << x << std::endl;


  //
  // check edit distance 
  //
  return hamming(x, target);

}