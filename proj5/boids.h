#ifndef BOIDS_H
#define BOIDS_H
#include "kdTree.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <Eigen/Dense>
#include <cmath>

using namespace std;

class Boids{
public:
    Boids(const string &fname);
    void details();
    Eigen::Vector3d moveTowardsCenter(vector<int> neighbors, unsigned int n);
    Eigen::Vector3d avoidCrowding(vector<int> neighbors, unsigned int n);
    Eigen::Vector3d matchVelocity(vector<int> neighbors, unsigned int n);
    bool outOfBounds(Eigen::Vector3d pt);
    void draw(const char * & fname);
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