CXX = g++
CXXFLAGS = -Wall -I/usr/include/eigen3

boids: kdTree.o
	$(CXX) $(CXXFLAGS) kdTree.o boids.cpp -o boids

kdTree.o:
	$(CXX) $(CXXFLAGS) -c kdTree.cpp

clean:
	rm *.o
	rm *~