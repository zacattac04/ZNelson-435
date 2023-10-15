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
