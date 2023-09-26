#ifndef RAYTRACE_H
#define RAYTRACE_H
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

struct Ray {
    Eigen::Vector3d e;
    Eigen::Vector3d d;
};

struct HitRecord {

    double t;

};

class Light {
public:
    Light(Eigen::Vector3d pos) {p = pos; c[0] = 1; c[1]=1; c[2]=1;}
    Light(Eigen::Vector3d pos, Eigen::Vector3d color) {p = pos; c = color;}
private:
    Eigen::Vector3d p;
    Eigen::Vector3d c;
};

class Surface {
public:
    Surface() {
        type = "null";
    }
    virtual void details() = 0;
    virtual bool hit(const Ray &r, double t0, double t1, HitRecord &hr) const = 0;
    void setFill(Fill f) {fill = f;}
    Fill getFill() {return fill;}
protected:
    string type;
    Fill fill;

};

class Triangle: public Surface{
public:
    Triangle();
    Triangle(Eigen::Vector3d& A, Eigen::Vector3d& B, Eigen::Vector3d& C);
    void details();
    bool hit(const Ray &r, double t0, double t1, HitRecord &hr) const;
private:
    Eigen::Vector3d a;
    Eigen::Vector3d b;
    Eigen::Vector3d c;
};

class Poly: public Surface{
public:
    Poly(vector<Eigen::Vector3d> vertices){ verts = vertices;}
    Eigen::Vector3d getVertex(int i) const {return verts[i];}
    void details();
    bool hit(const Ray &r, double t0, double t1, HitRecord &hr) const;
private:
    vector<Eigen::Vector3d> verts;
};

class Sphere: public Surface{
public:
    Sphere(Eigen::Vector3d center, double r){ c = center; radius = r;}
    void details();
    bool hit(const Ray &r, double t0, double t1, HitRecord &hr) const;

private:
    Eigen::Vector3d c;
    double radius;
};

class PolyPatch: public Surface {
public:
    PolyPatch(vector<Eigen::Vector3d> vertices, vector<Eigen::Vector3d> normals){ verts = vertices; norms = normals; }
    Eigen::Vector3d getVertex(int i) const {return verts[i];}
    Eigen::Vector3d getNormal(int i) const {return norms[i];}
    void details();
    bool hit(const Ray &r, double t0, double t1, HitRecord &hr) const;
private:
    vector<Eigen::Vector3d> verts;
    vector<Eigen::Vector3d> norms;
};



class Tracer {
public:
    Tracer(const string &fname);
    ~Tracer();
    Eigen::Vector3d castRay(const Ray &r, double t0, double t1) const;
    void createImage(const char * &fname);
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
    vector<Surface*> surfaces;
};

#endif