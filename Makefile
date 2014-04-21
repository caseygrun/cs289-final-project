ga: ga.cpp ga.hpp
	clang++ -ggdb ga.cpp -o ga

testing: testing.cpp
	clang++ -ggdb testing.cpp -o testing

clean:
	rm ga
