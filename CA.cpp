# include <cstdlib>
# include <iostream>
# include <ctime>

using namespace std;

// Cellular Automata
// Alex Garruss and Casey Grun
// CS289 April 2014

// GNU GPL

// some code borrowed from
// http://people.sc.fsu.edu/~jburkardt/cpp_src/cellular_automaton/cellular_automaton.html
// Ref: http://en.wikibooks.org/wiki/Algorithm_Implementation/Strings/Levenshtein_distance#C

// Spiritual reference
// Stephen Wolfram,A New Kind of Science,Wolfram Media, 2002,ISBN13: 978-1579550080 



int main ( );
int levenshtein(const char *s, int ls, const char *t, int lt);
void timestamp ( );


int main ( )

{
  int i;
  int j;
  int n;
  int step_num;
  char *x;
  char *x_old;

//  timestamp ( );

// n is the columns

  n = 100;

// step_num are the rows
  
  step_num = 50;

  x = new char[n+2];
  x_old = new char[n+2];

//   for ( i = 0; i <= n + 1; i++ )
//   {
//     x[i] = '0';
//   }



// //  initial condition
//   x[n-1] = '1';

  string xp = "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001";
//1101101011001001110111000110101100100001010011101011110111110110011101001001110101001111001000001110";
  for (int i = 0; i < n; ++i)
  {
    x[i] = xp[i];
  }


  for ( i = 1; i <= n; i++ )
  {
    cout << x[i];
  }
  cout << "\n";

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


	if ( ( x_old[i-1] == '0' && x_old[i] == '0' && x_old[i+1] == '1' ) ||
           ( x_old[i-1] == '0' && x_old[i] == '1' && x_old[i+1] == '0' ) ||
           ( x_old[i-1] == '0' && x_old[i] == '1' && x_old[i+1] == '1' ) ||
           ( x_old[i-1] == '1' && x_old[i] == '0' && x_old[i+1] == '1' ) ||
           ( x_old[i-1] == '1' && x_old[i] == '1' && x_old[i+1] == '0' ) )
        {
        	x[i] = '1';
	}

      

	else
        {
        	x[i] = '0';
        }
    }
//
//  Enforce periodic boundary conditions.
//
// uncomment to turn on
//    x[0] = x[n];
//    x[n+1] = x[1];


    for ( i = 1; i <= n; i++ )
    {
      cout << x[i];
    }
    cout << "\n";
  }


//
// check edit distance 
//

// call levenshtein
//int cur_dist = levenshtein(x,n+2,x_old,n+2);
//cout << cur_dist;


//
//  Free memory.
//
  delete [] x;
  delete [] x_old;
//
//  Terminate.
//
  cout << "\n";
  //cout << "\n";
  //timestamp ( );

  return 0;
}


int levenshtein(const char *s, int ls, const char *t, int lt)
{
        int a, b, c;

        if (!ls) return lt;
        if (!lt) return ls;

        if (s[ls] == t[ls])
                return levenshtein(s, ls - 1, t, lt - 1);

        a = levenshtein(s, ls - 1, t, lt - 1);
        b = levenshtein(s, ls,     t, lt - 1);
        c = levenshtein(s, ls - 1, t, lt    );

        if (a > b) a = b;
        if (a > c) a = c;

        return a + 1;
}



void timestamp ( )

{
# define TIME_SIZE 40

  static char time_buffer[TIME_SIZE];
  const struct std::tm *tm_ptr;
  size_t len;
  std::time_t now;

  now = std::time ( NULL );
  tm_ptr = std::localtime ( &now );

  len = std::strftime ( time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm_ptr );

  std::cout << time_buffer << "\n";

  return;
# undef TIME_SIZE
}
