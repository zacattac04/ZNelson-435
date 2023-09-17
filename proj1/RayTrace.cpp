#include "RayTrace.h"
#include <sstream>
#include <fstream>


// Returns the determinant of the 3x3 matrix
double det(const Eigen::Vector3d &a, const Eigen::Vector3d &b, const Eigen::Vector3d &c) {
    return  a[0] * (b[1] * c[2] - c[1] * b[2]) +
            b[0] * (c[1] * a[2] - a[1] * c[2]) +
            c[0] * (a[1] * b[2] - b[1] * a[2]);
}

// Reads all of the data from the nff file
Tracer::Tracer(const string &fname) {
    ifstream istream(fname.c_str() ,ios_base::in);
    string line;
    char ch;
    int i = 0;
    while (!istream.eof()){
        getline(istream, line);
        switch (line[0]) {
            case 'b': {
                stringstream ss(line);
                ss>>ch>>bcolor[0]>>bcolor[1]>>bcolor[2];
                break;
            }
            case 'v': {
                getline(istream, line);
                string junk;
                stringstream fromss(line);
                fromss>>junk>>eye[0]>>eye[1]>>eye[2];
                
                getline(istream, line);
                stringstream atss(line);
                atss>>junk>>at[0]>>at[1]>>at[2];

                getline(istream, line);
                stringstream upss(line);
                upss>>junk>>up[0]>>up[1]>>up[2];

                getline(istream, line);
                stringstream angless(line);
                angless>>junk>>angle;

                getline(istream, line);
                stringstream hitherss(line);
                hitherss>>junk>>hither;

                getline(istream,line);
                stringstream resss(line);
                resss>>junk>>res[0]>>res[1];
                break;
            }

            case 'f': {
                stringstream ss(line);
                ss>>ch>>fill.color[0]>>fill.color[1]>>fill.color[2]>>fill.kd>>fill.ks>>fill.shine>>fill.t>>fill.ior;
                break;
            }
            case 'p': {
                stringstream ss(line);
                int numCoords;
                ss>>ch>>numCoords;
                if (numCoords == 3) {
                    getline(istream, line);
                    stringstream ass(line);
                    Eigen::Vector3d A;
                    ass>>A[0]>>A[1]>>A[2];

                    getline(istream, line);
                    stringstream bss(line);
                    Eigen::Vector3d B;
                    bss>>B[0]>>B[1]>>B[2];

                    getline(istream, line);
                    stringstream css(line);
                    Eigen::Vector3d C;
                    css>>C[0]>>C[1]>>C[2];
                    //Triangle* triangle = new Triangle(A, B, C);
                    surfaces.push_back(new Triangle(A, B, C));
                }
                break;
            }
            default: {
                break;
            }
        }
    }
    istream.close();
}

// Deletes all of the surface objects
Tracer::~Tracer(){
    for (int i = 0; i<surfaces.size(); i++) {
        delete surfaces[i];
    }
}

// Casts a Ray, and checks every object to see if it exists within its path
// Currently, the only objects that exists are triangles, but this code is set up to expand to other surfaces
Eigen::Vector3d Tracer::castRay(const Ray &r, double t0, double t1) const {
    HitRecord hr;
    Eigen::Vector3d color(bcolor);

    bool hit = false;
    for (int i = 0; i<surfaces.size(); i++) {
        if  (surfaces[i]->hit(r, t0, t1, hr)) {
            hit = true;
        }
    }
    if (hit)
        color = fill.color;
    return color;
}

// Sets up the camera and casts a ray for every pixel of the image
// It then adds the pixel to the image, which it exports at the end
// Currently, this is only set up to change the color if the ray had an intersection
// It doesn't really care if there is more than one intersection, since all surfaces are of the same type and color
// This will have to be rectified in the future 
void Tracer::createImage(const string &fname) {
    Eigen::Vector3d w = eye - at;
    w /= w.norm();
    Eigen::Vector3d u = up.cross(w);
    u.normalize();
    Eigen::Vector3d v = w.cross(u);
    //cout << "w: \n" << w << " u: \n" << u << "v: \n" << v << endl;
    double d = (eye - at).norm();
    double h = tan((M_PI/180.0) * (angle/2.0)) * d;
    double increment = (2*h) / res[0];
    double l = -h + 0.5*increment;
    double t = h*(((double)res[1])/res[0]) - 0.5*increment;
    int height = (int) res[1];
    int width = (int) res[0];
    unsigned char pixels[height][width][3];
    int progressCounter = 0;
    int progressInterval  = (height*width)/25;
    cout << "Drawing Image:" << endl;
    cout << "-------------------------" << endl;
    for (int i = 0; i <res[1]; i++){
        for (int j = 0; j<res[0]; j++) {
            double x = l + j*increment;
            double y = t - i*increment;
            Eigen::Vector3d dir = x*u + y*v - d*w;
            Eigen::Vector3d pixel;
            Ray r;
            r.e = eye;
            r.d = dir;
            r.d.normalize();
            pixel = castRay(r, hither, 1000);
            pixels[i][j][0] = pixel[0] * 255;
            pixels[i][j][1] = pixel[1] * 255;
            pixels[i][j][2] = pixel[2] * 255;
            progressCounter++;
            if (progressCounter % progressInterval == 0) {
                cout << "*";
            }
        }
    }
    cout << endl << "done!" << endl;
    FILE *f = fopen("hide.ppm","wb");
    fprintf(f, "P6\n%d %d\n%d\n", width, height, 255);
    fwrite(pixels, 1, height*width*3, f);
    fclose(f);

}


// A class that prints all of the details read from the nff file
// Isn't utilized in the current code, was just to check if the file was being read properly
void Tracer::details(){
    cout << "Background Color:" << endl;
    cout << "R: " <<bcolor[0] << "\tB: " << bcolor[1] << "\tG: " << bcolor[2] << endl << endl;
    cout << "Viewpoint details:" << endl;
    cout << "Eye: " << eye[0] << "\t" << eye[1] << "\t" << eye[2] << endl;
    cout << "At: " << at[0] << "\t" << at[1] << "\t" << at[2] << endl;
    cout << "Up: " << up[0] << "\t" << up[1] << "\t" << up[2] << endl;
    cout << "Angle: " << angle << endl;
    cout << "Hither: " << hither << endl;
    cout << "Resolution: " << res[0] << "\t" << res[1] << endl << endl;
    cout << "Fill Details: " << endl;
    cout << "R: " << fill.color[0] << "\tB: " << fill.color[1] << "\tG: " << fill.color[2] << endl << endl;
    cout << "Surfaces:" << endl;
    
    for (int i = 0; i < surfaces.size(); i++) {
        surfaces[i]->details();
    }

}




// Shows the details of the triangle object
void Triangle::details() {
    cout << "Type: Triangle" << endl;
    cout << a[0] << "\t" << a[1] << "\t" << a[2] << endl;
    cout << b[0] << "\t" << b[1] << "\t" << b[2] << endl;
    cout << c[0] << "\t" << c[1] << "\t" << c[2] << endl;
}

// Simple constructor for the Triangle class. probably could be done better
Triangle::Triangle(Eigen::Vector3d& A, Eigen::Vector3d& B, Eigen::Vector3d& C) {
    a = A;
    b = B;
    c = C;
    type = "Triangle";

}

// Checks to see if the Ray hits this Triangle
// If it was hit, it records the details in the HitRecord
bool Triangle::hit(const Ray &r, double t0, double t1, HitRecord &hr) const {
    Eigen::Vector3d ba = a-b;
    Eigen::Vector3d ca = a-c;
    Eigen::Vector3d ea = a-r.e;
    double detA = det(ba, ca, r.d);
    double beta = det(ea, ca, r.d)/detA;
    if (beta < 0 || beta > 1)
        return false;
    double gamma = det(ba, ea, r.d)/detA;
    if(gamma < 0.0 || gamma > 1.0-beta)
        return false;
    double t = det(ba, ca, ea)/detA;
    if(t < t0 || t > t1)
        return false;

    hr.t = t;
    hr.p = r.e + t * r.d;
    hr.n = ba.cross(ca);
    hr.n.normalize();
    hr.alpha = 1.0 - beta - gamma;
    hr.beta = beta;
    hr.gamma = gamma;
    return true;

}

int main(int argc, const char * argv[]) {
    Tracer tracer(argv[1]);
    //tracer.details();
    //cout << "Drawing image:" << endl;
    tracer.createImage(argv[2]);
    return 0;
}
