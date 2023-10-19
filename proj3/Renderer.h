#ifndef RENDERER_H
#define RENDERER_H
#include <iostream>
#include <stdio.h>
#include <string>
#include <Eigen/Dense>

using namespace std;


struct Fill {
    Eigen::Vector3d color;
    double kd;
    double ks;
    double e;
    double kt;
    double ir;
};

struct Light {
    Eigen::Vector3d p;
    Eigen::Vector3d c;
};

struct Fragment {
    double z;
    Eigen::Vector3d color;
};

class Triangle{
public:
    Triangle();
    Triangle(Eigen::Vector3d& A, Eigen::Vector3d& B, Eigen::Vector3d& C) {a = A; b = B; c = C; na = {0.0,0.0,0.0}; nb = {0.0,0.0,0.0}; nc = {0.0,0.0,0.0};};
    Triangle(Eigen::Vector3d& A, Eigen::Vector3d& B, Eigen::Vector3d& C, Eigen::Vector3d &NA, Eigen::Vector3d &NB, Eigen::Vector3d &NC) {a = A; b = B; c = C; na = NA; nb = NB; nc = NC;};
    void details();
    void setFill(Fill f) {fill = f;}
    Fill getFill() {return fill;}

    Eigen::Vector3d a;
    Eigen::Vector3d b;
    Eigen::Vector3d c;
    Eigen::Vector3d na;
    Eigen::Vector3d nb;
    Eigen::Vector3d nc;
    Eigen::Vector4d p[3];
    Eigen::Vector3d color[3];
    Fill fill;
};

class Renderer {
public:
    Renderer(const string &fname);
    ~Renderer();
    void vertexProcessing(bool shading);
    Eigen::Vector3d shade(Eigen::Vector3d p, Eigen::Vector3d n, Fill f);
    void rasterizer(const Triangle &t);
    void details();
    bool checkExtension(const string fname, string extension);

private:
    Eigen::Vector3d eye;
    Eigen::Vector3d at;
    Eigen::Vector3d up;
    double angle;
    double hither;
    Eigen::Vector2d res;
    Eigen::Vector3d bcolor;
    vector<Triangle*> triangles;
    vector<Light> lights;
    Eigen::Matrix4d M_vp;
    Eigen::Matrix4d M_per;
    Eigen::Matrix4d M_cam;
    Eigen::Matrix4d M;
    vector<Fragment> *fragments;
};

#endif