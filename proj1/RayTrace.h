#ifndef RAYTRACE_H
#define RAYTRACE_H
#include <iostream>
#include <stdio.h>
#include <string>
#include <Eigen/Dense>

using namespace std;


struct Ray {
    Eigen::Vector3d e;
    Eigen::Vector3d d;
};

struct HitRecord {
    Eigen::Vector3d p;
    Eigen::Vector3d n;
    Eigen::Vector3d v;
    double t;
    double alpha;
    double beta;
    double gamma;
};

class Surface {
public:
    Surface() {
        type = "null";
    }
    virtual void details() = 0;
    virtual bool hit(const Ray &r, double t0, double t1, HitRecord &hr) const = 0;
protected:
    string type;

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

/*
class HitRecord {
public:
    HitRecord();
private:
    Surface s;
    double t;
    Eigen::Vector3d n;

};
*/
struct Fill {
    Eigen::Vector3d color;
    double kd;
    double ks;
    double shine;
    double t;
    double ior;
};


class Tracer {
public:
    Tracer(const string &fname);
    ~Tracer();
    Eigen::Vector3d castRay(const Ray &r, double t0, double t1) const;
    void createImage(const string &fname);
    void details();

private:
    Eigen::Vector3d eye;
    Eigen::Vector3d at;
    Eigen::Vector3d up;
    double angle;
    double hither;
    Eigen::Vector2d res;
    Eigen::Vector3d bcolor;
    Fill fill;
    vector<Surface*> surfaces;
};

#endif