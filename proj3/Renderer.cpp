#include "Renderer.h"
#include <sstream>
#include <fstream>

// Returns the determinant of the 3x3 matrix
double det(const Eigen::Vector3d &a, const Eigen::Vector3d &b, const Eigen::Vector3d &c) {
    return  a[0] * (b[1] * c[2] - c[1] * b[2]) +
            b[0] * (c[1] * a[2] - a[1] * c[2]) +
            c[0] * (a[1] * b[2] - b[1] * a[2]);
}

// Reads all of the data from the nff file
// All surfaces (Polygons, Polygon patches, spheres) are placed in the surfaces vector
// Light objects are placed in the light vector
// The fill is stored temporarily, and used to initialize the fill of all proceeding surfaces
Renderer::Renderer(const string &fname) {
    bool valid = checkExtension(fname, ".nff");
    if (!valid)
        throw runtime_error("Invalid input file extension");
    ifstream istream(fname.c_str(), ios_base::in);
    string line;
    char ch;
    int i = 0;
    Fill fill;
    /*
    M_cam = Eigen::Matrix4d::Zero();
    M_per = Eigen::Matrix4d::Zero();
    M_vp = Eigen::Matrix4d::Zero();
    M = Eigen::Matrix4d::Zero();
    */
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

                M_vp << res[0] / 2, 0, 0, (res[0]-1)/2,
                        0, res[1] / 2, 0, (res[1]-1)/2,
                        0, 0, 1, 0,
                        0, 0, 0, 1;
                Eigen::Vector3d w = eye - at;
                w /= w.norm();
                Eigen::Vector3d u = up.cross(w);
                u.normalize();
                Eigen::Vector3d v = w.cross(u);
                M_cam << u[0], u[1], u[2], 0,
                         v[0], v[1], v[2], 0,
                         w[0], w[1], w[2], 0,
                         0, 0, 0, 1;
                Eigen::Matrix4d M_inv;
                M_inv << 1, 0, 0, -1 * eye[0],
                         0, 1, 0, -1 * eye[1],
                         0, 0, 1, -1 * eye[2],
                         0, 0, 0, 1;
                M_cam = M_cam * M_inv;

                double d = (eye - at).norm();
                double h = tan((M_PI/180.0) * (angle/2.0)) * d;
                double increment = (2*h) / res[0];
                double l = -h + 0.5*increment;
                double t = h*(((double)res[1])/res[0]) - 0.5*increment;
                double r = h - 0.5*increment;
                double b = -h*(((double)res[1])/res[0]) + 0.5*increment;
                double f = 10000;
                M_per << (2 * hither) / (r-l), 0, (l+r) / (r-l), 0,
                         0, (2 * hither) / (t-b), (b+t) / (b-t), 0,
                         0, 0, (f + hither) / (hither - f), (2 * f * hither) / (f - hither),
                         0, 0, 1, 0;
                M = M_vp * M_per * M_cam;
                break;
            }

            case 'f': {
                stringstream ss(line);
                ss>>ch>>fill.color[0]>>fill.color[1]>>fill.color[2]>>fill.kd>>fill.ks>>fill.e>>fill.kt>>fill.ir;
                break;
            }
            case 'p': {
                if (line[1] == 'p') {
                    stringstream ss(line);
                    int numCoords;
                    string junk;
                    ss>>junk>>numCoords;
                    vector<Eigen::Vector3d> verts;
                    vector<Eigen::Vector3d> norms;
                    for (int i = 0; i < numCoords; i++) {
                        getline(istream, line);
                        stringstream ss(line);
                        Eigen::Vector3d v;
                        Eigen::Vector3d n;
                        ss>>v[0]>>v[1]>>v[2]>>n[0]>>n[1]>>n[2];
                        verts.push_back(v);
                        norms.push_back(n);
                    }
                    for (int i = 2; i < verts.size(); i++) {
                        Triangle* triangle = new Triangle(verts[0], verts[i-1], verts[i]);
                        triangle->setFill(fill);
                        triangles.push_back(triangle);
                    }
                } else {
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
                        triangles.push_back(new Triangle(A,B,C));
                        triangles.back()->setFill(fill);
                    } else if (numCoords > 3) {
                        vector<Eigen::Vector3d> verts;
                        for (int i = 0; i < numCoords; i++) {
                            getline(istream, line);
                            stringstream ss(line);
                            Eigen::Vector3d v;
                            ss>>v[0]>>v[1]>>v[2];
                            verts.push_back(v);
                        }
                        for (int i = 2; i < verts.size(); i++) {
                            Triangle* triangle = new Triangle(verts[0], verts[i-1], verts[i]);
                            triangle->setFill(fill);
                            triangles.push_back(triangle);
                        }

                    }
                }
                break;
            }
            case 'l': {
                stringstream ss(line);
                Eigen::Vector3d p;
                ss>>ch>>p[0]>>p[1]>>p[2];
                Light l;
                l.p = p;
                lights.push_back(l);
            }
            default: {
                break;
            }
        }
    }
    istream.close();
}

// Deletes all of the surface objects
Renderer::~Renderer(){
    for (int i = 0; i<triangles.size(); i++) {
        delete triangles[i];
    }
}

// A class that prints all of the details read from the nff file
// Isn't utilized in the current code, was just to check if the file was being read properly
void Renderer::details(){
    cout << "Background Color:" << endl;
    cout << "R: " <<bcolor[0] << "\tB: " << bcolor[1] << "\tG: " << bcolor[2] << endl << endl;
    cout << "Viewpoint details:" << endl;
    cout << "Eye: " << eye[0] << "\t" << eye[1] << "\t" << eye[2] << endl;
    cout << "At: " << at[0] << "\t" << at[1] << "\t" << at[2] << endl;
    cout << "Up: " << up[0] << "\t" << up[1] << "\t" << up[2] << endl;
    cout << "Angle: " << angle << endl;
    cout << "Hither: " << hither << endl;
    cout << "Resolution: " << res[0] << "\t" << res[1] << endl << endl;
    cout << "Number of triangles: " << triangles.size() << endl;

    double d = (eye - at).norm();
    double h = tan((M_PI/180.0) * (angle/2.0)) * d;
    double increment = (2*h) / res[0];
    double l = -h + 0.5*increment;
    double t = h*(((double)res[1])/res[0]) - 0.5*increment;
    cout << "d: " << d << endl;
    cout << "h: " << h << endl;
    cout << "increment: " << increment << endl;
    cout << "l: " << l << endl;
    cout << "t: " << t << endl;
    cout << "M_vp: \n" << M_vp << endl;
    cout << "M_cam: \n" << M_cam << endl;
    cout << "M_per: \n" << M_per << endl;
    cout << "M: \n" << M << endl;
    
} 

// Shows the details of the triangle object
void Triangle::details() {
    cout << "Type: Triangle" << endl;
    cout << "Color: " << endl;
    cout << "R: " << fill.color[0] << "\tB: " << fill.color[1] << "\tG: " << fill.color[2] << endl << endl;
    //cout << a[0] << "\t" << a[1] << "\t" << a[2] << endl;
    //cout << b[0] << "\t" << b[1] << "\t" << b[2] << endl;
    //cout << c[0] << "\t" << c[1] << "\t" << c[2] << endl;
}

// A simple function to check if the file name extension is valid
// Returns true if the extension is correct, false otherwise
bool Renderer::checkExtension(const string fname, string extension) {
    int index = fname.find('.');
    if (index == -1)
        return false;
    if (fname.substr(index).compare(extension) != 0) {
        return false;
    }
    return true;
}

int main(int argc, const char * argv[]) {
    if (argv[1] == nullptr) {
        throw runtime_error("No input file given");
    }
    Renderer renderer(argv[1]);
    renderer.details();
    return 0;
}
