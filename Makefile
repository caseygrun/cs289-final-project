ga: ga.cpp ga.hpp ca_engine.cpp ca_engine.hpp
	clang++ -ggdb ca_engine.cpp ga.cpp -o ga


CA: CA.cpp
	g++ CA.cpp -o CA

clean:
	rm ga
