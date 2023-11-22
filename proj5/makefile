CXX = g++
CXXFLAGS = -Wall -I C:\Users\zacha\Documents\eigen-3.4.0\

boids: kdTree.o
	$(CXX) $(CXXFLAGS) kdTree.o boids.cpp -o boids

kdTree.o:
	$(CXX) $(CXXFLAGS) -c kdTree.cpp

clean:
	rm *.o*
	rm *~