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

class Triangle{
public:
    Triangle();
    Triangle(Eigen::Vector3d& A, Eigen::Vector3d& B, Eigen::Vector3d& C) {a = A; b = B; c = C;};
    void details();
    void setFill(Fill f) {fill = f;}
    Fill getFill() {return fill;}
private:
    Eigen::Vector3d a;
    Eigen::Vector3d b;
    Eigen::Vector3d c;
    Fill fill;
};

class Renderer {
public:
    Renderer(const string &fname);
    ~Renderer();
    //Eigen::Vector3d castRay(const Ray &r, double t0, double t1) const;
    //bool castShadow(const Ray &r, double t0, double t1) const;
    //Eigen::Vector3d shade(const HitRecord &hr) const;
    //void createImage(const char * &fname);
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
};

#endif