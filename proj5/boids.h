#ifndef BOIDS_H
#define BOIDS_H
//#include "kdTree.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <Eigen/Dense>

using namespace std;

class Boids{
public:
    Boids(const string &fname);
    void details();
private:
    double size;
    double neighbor_radius;
    int num_neighbors;
    double mass;
    double collision;
    double centering;
    double velocity;
    double hunger;
    double damping;
    double dt;
    double length;
    int nboids;
    int nfood;
    vector<Eigen::Vector3d> pts;
    vector<Eigen::Vector3d> ptsV;
};




#endif