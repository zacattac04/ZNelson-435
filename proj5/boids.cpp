#include "boids.h"
#include <sstream>
#include <fstream>


Boids::Boids(const string &fname) {
    ifstream istream(fname.c_str(), ios_base::in);
    string line;
    char ch;
    int i = 0;
    string empty = "";
    getline(istream, line);
    stringstream ss(line);
    ss>>size>>neighbor_radius>>num_neighbors>>mass>>collision>>centering>>velocity>>hunger>>damping>>dt>>length;

    getline(istream, line);
    stringstream nbss(line);
    nbss>>nboids;

    while (i < nboids) {
        getline(istream, line);
        if (line.compare(empty) != 0) {
            stringstream bss(line);
            Eigen::Vector3d position;
            Eigen::Vector3d velocity;
            char dummy;
            bss>>dummy>>position[0]>>dummy>>position[1]>>dummy>>position[2]>>dummy;
            bss>>dummy>>velocity[0]>>dummy>>velocity[1]>>dummy>>velocity[2]>>dummy;
            pts.push_back(position);
            ptsV.push_back(velocity);
            i++;
        }
    }
    getline(istream, line);
    getline(istream, line);
    stringstream nfss(line);
    nfss>>nfood;
    i = 0;
    while (i < nfood) {
        getline(istream, line);
        if (line.compare(empty) != 0) {
            stringstream fss(line);
            Eigen::Vector3d position;
            Eigen::Vector3d velocity;
            float time;
            char dummy;
            fss>>dummy>>position[0]>>dummy>>position[1]>>dummy>>position[2]>>dummy;
            fss>>dummy>>velocity[0]>>dummy>>velocity[1]>>dummy>>velocity[2]>>dummy;
            fss>>time;
            i++;
        }
    }
    istream.close();
}

void Boids::details() {
    cout << "Size: " << size << endl;
    cout << "Neighbor Radius: " << neighbor_radius << endl;
    cout << "Num Neighbors: " << num_neighbors << endl;
    cout << "Mass: " << mass << endl;
    cout << "Collision: " << collision << endl;
    cout << "Cenetering: " << centering << endl;
    cout << "Velocity: " << velocity << endl;
    cout << "Hunger: " << hunger << endl;
    cout << "Damping: " << damping << endl;
    cout << "Dt: " << dt << endl;
    cout << "Length: " << length << endl;
    
    cout << "nboids: " << nboids << endl;
    for (int i = 0; i < 10; i++) {
        Eigen::Vector3d pos = pts[i];
        Eigen::Vector3d v = ptsV[i];

        cout << "[" << pos[0] << "," << pos[1] << "," << pos[2] << "] ";
        cout << "[" << v[0] << "," << v[1] << "," << v[2] << "] " << endl;
    }
} 

int main(int argc, const char * argv[]) {
    if (argc < 2) {
        throw runtime_error("No input file given");
    }
    Boids boids(argv[1]);
    boids.details();

    return 0;
}