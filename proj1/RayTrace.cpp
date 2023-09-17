#include "RayTrace.h"
#include <sstream>
#include <fstream>


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

double det(const Eigen::Vector3d &a, const Eigen::Vector3d &b, const Eigen::Vector3d &c) {
    return  a[0] * (b[1] * c[2] - c[1] * b[2]) +
            b[0] * (c[1] * a[2] - a[1] * c[2]) +
            c[0] * (a[1] * b[2] - b[1] * a[2]);
}

void Triangle::details() {
    cout << "Type: Triangle" << endl;
    cout << a[0] << "\t" << a[1] << "\t" << a[2] << endl;
    cout << b[0] << "\t" << b[1] << "\t" << b[2] << endl;
    cout << c[0] << "\t" << c[1] << "\t" << c[2] << endl;
}

Triangle::Triangle(Eigen::Vector3d& A, Eigen::Vector3d& B, Eigen::Vector3d& C) {
    a = A;
    b = B;
    c = C;
    type = "Triangle";

}
int main(int argc, const char * argv[]) {
    Tracer tracer(argv[1]);
    cout << "printing details:" << endl;
    tracer.details();
    return 0;
}
