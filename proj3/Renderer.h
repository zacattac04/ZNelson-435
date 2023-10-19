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
    Triangle(Eigen::Vector4d& A, Eigen::Vector4d& B, Eigen::Vector4d& C) {a = A; b = B; c = C;};
    Triangle(Eigen::Vector4d& A, Eigen::Vector4d& B, Eigen::Vector4d& C, Eigen::Vector4d &NA, Eigen::Vector4d &NB, Eigen::Vector4d &NC) {a = A; b = B; c = C; na = NA; nb = NB; nc = NC;};
    void details();
    void setFill(Fill f) {fill = f;}
    Fill getFill() {return fill;}
private:
    Eigen::Vector4d a;
    Eigen::Vector4d b;
    Eigen::Vector4d c;
    Eigen::Vector4d na;
    Eigen::Vector4d nb;
    Eigen::Vector4d nc;
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