#include "RayTrace.h"


Tracer::Tracer(const string &fname) {
    ifstream istream(fname.c_str(), std::ios_base);
    string line;
    char ch;

    while (istream.is_open()){
        getLine(istream, line);
        switch (line[0]) {
            case 'b': {
                stringstream ss(line);
                ss>>ch>>bcolor[0]>>bcolor[1]>>bcolor[2];
                break;
            }
            case 'v': {
                getLine(istream, line);
                string junk;
                stringstream fromss(line);
                fromss>>junk>>eye[0]>>eye[1]>>eye[2];
                
                getLine(istream, line);
                stringstream atss(line);
                atss>>junk>>at[0]>>at[1]>>at[2];

                getLine(istream, line);
                stringstream upss(line);
                upss>>junk>>up[0]>>up[1]>>up[2];

                getLine(istream, line);
                stringstream angless(line);
                angless>>junk>>angle;

                getLine(istream, line);
                stringstream hitherss(line);
                hitherss>>junk>>hither;

                getLine(istream,line);
                stringstream resss(line);
                resss>>junk>>res[0]>>res[1];
                break;
            }

            case 'l': {
                break;
            }

            case 'f': {
                stringstream ss(line);
                ss>>fill.color[0]>>fill.color[1]>>fill.color[2]>>fill.kd>>fill.ks>>fill.shine>>fill.t>>fill.ior;
                break;
            }
            case 'p': {
                stringstream ss(line);
                int numCoords;
                ss>>ch>>numCoords;
                if (numCoords == 3) {
                    Triangle triangle;

                    getLine(iStream, line);
                    stringstream ass(line);
                    ass>>triangle.a[0]>>triangle.a[1]>>triangle.a[2];

                    getLine(iStream, line);
                    stringstream bss(line);
                    bss>>triangle.b[0]>>triangle.b[1]>>triangle.b[2];

                    getLine(iStream, line);
                    stringStream css(line);
                    css>>triangle.c[0]>>triangle.c[1]>>triangle.c[2];

                    surfaces.insert(triangle);
                }
            }
        }
    }
}

Tracer::details(){
    cout << "Background Color:" << endl;
    cout << "R: " <<bcolor[0] << "\tB: " << bcolor[1] << "\tG: " << bcolor[2] << endl << endl;
    cout << "Viewpoint details:" << endl;
    cout << "Eye: " << eye[0] << "\t" << eye[1] << "\t" << eye[2] << endl:
    cout << "At: " << at[0] << "\t" << at[1] << "\t" << at[2] << endl;
    cout << "Up: " << up[0] << "\t" << up[1] << "\t" << up[2] << endl;
    cout << "Angle: " << angle << endl;
    cout << "Hither: " << hither << endl;
    cout << "Resolution: " << res[0] << "\t" << res[1] << endl << endl;
    cout << "Fill Details: " << endl;
    cout << "R: " << fill.color[0] << "\tB: " << fill.color[1] << "\tG: " << fill.color[2] << endl << endl;
    cout << "Surfaces:" << endl;
    
    for (int i = 0; i < surfaces.size(); i++) {

    }

}

double det(const Eigen::Vector3d &a, const Eigen::Vector3d &b, const Eigen::Vector3d &c) {
    return  a[0] * (b[1] * c[2] - c[1] * b[2]) +
            b[0] * (c[1] * a[2] - a[1] * c[2]) +
            c[0] * (a[1] * b[2] - b[1] * a[2]);
}

Triangle::details() {
    cout << "Type: Triangle";
    cout << a[0] << "\t" << a[1] << "\t" << a[2] << endl;
    cout << b[0] << "\t" << b[1] << "\t" << b[2] << endl;
    cout << c[0] << "\t" << c[1] << "\t" << c[2] << endl;
}

int main(int argc, const char * argv[]) {
    Tracer tracer(argv[optind++]);
    tracer.details();
    return 0;
}