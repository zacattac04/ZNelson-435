#include <sstream>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <Eigen/Dense>

using namespace std;

int main() {
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
    string line = "[-0.499992,-0.184231,0.0639013] [-0.00413499,0.00327672,-0.0281041]";
    stringstream ss(line);
    Eigen::Vector3d position;
    Eigen::Vector3d velocity;
    char dummy;
    ss>>dummy>>position[0]>>dummy>>position[1]>>dummy>>position[2]>>dummy;
    ss>>dummy>>velocity[0]>>dummy>>velocity[1]>>dummy>>velocity[2]>>dummy;

    cout << "Position: " << position[0] << " " << position[1] << " " << position[2] << endl;
    cout << "Velocity: " << velocity[0] << " " << velocity[1] << " " << velocity[2] << endl;
    cout << "Dummy: " << dummy << endl;
    
    return 0;    
}