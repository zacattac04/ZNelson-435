#include <sstream>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
//#include <Eigen/Dense>

using namespace std;

int main(int argc, const char * argv[]) {
    /*
    string fname = "sample.in";
    ifstream istream(fname.c_str(), ios_base::in);
    string line;
    string empty = "";
    int numEmpty = 0;
    while(!istream.eof()){
        getline(istream, line);
        if (line.compare(empty)==0) 
            numEmpty++;
    }
    cout << "Number of empty Lines: " << numEmpty << endl;
    */
    FILE *f = fopen(argv[1], "wb");
    fprintf(f, "I am writing to a file\n");
    fclose(f);
    return 0;    
}