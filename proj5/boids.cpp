#include "boids.h"
#include <sstream>
#include <fstream>


Boids::Boids(const string &fname) {
    ifstream istream(fname.c_str(), ios_base::in);
    string line;
    int i = 0;
    string empty = "";
    getline(istream, line);
    stringstream ss(line);
    ss>>size>>neighbor_radius>>num_neighbors>>mass>>collision>>centering>>velocity>>hunger>>damping>>dt>>length;

    getline(istream, line);
    stringstream nbss(line);
    nbss>>nboids;

    while (i < nboids) {
        getline(istream, line);
        if (line.compare(empty) != 0) {
            stringstream bss(line);
            Eigen::Vector3d position;
            Eigen::Vector3d velocity;
            char ch;
            bss>>ch>>position[0]>>ch>>position[1]>>ch>>position[2]>>ch;
            bss>>ch>>velocity[0]>>ch>>velocity[1]>>ch>>velocity[2]>>ch;
            pts.push_back(position);
            ptsV.push_back(velocity);
            i++;
        }
    }
    getline(istream, line);
    getline(istream, line);
    stringstream nfss(line);
    nfss>>nfood;
    i = 0;
    // None of this data is being properly stored, so it won't be used
    while (i < nfood) {
        getline(istream, line);
        if (line.compare(empty) != 0) {
            stringstream fss(line);
            Eigen::Vector3d position;
            Eigen::Vector3d velocity;
            float time;
            char ch;
            fss>>ch>>position[0]>>ch>>position[1]>>ch>>position[2]>>ch;
            fss>>ch>>velocity[0]>>ch>>velocity[1]>>ch>>velocity[2]>>ch;
            fss>>time;
            i++;
        }
    }
    istream.close();
}

void Boids::draw(const char * & fname) {
    FILE * f = fopen(fname, "wb");
    int steps = (int)ceil(length / dt);
    int v1strong = 0;
    int v2strong = 0;
    int v3strong = 0;
    int v1small = 0;
    double v1avg = 0;
    double v2avg = 0;
    double v3avg = 0;
    //int steps = (int)length;
    fprintf(f, "%d\n", steps);
    cout << "Drawing animation: Steps out of " << steps << endl;
    for (int i = 0; i < steps; i++) {
        if (i % 25 == 0) {
            cout << i << endl;
        }
        fprintf(f, "%lld\n", pts.size());
        vector<Eigen::Vector3d> newPts;
        vector<Eigen::Vector3d> newPtsV;
        for (unsigned int k = 0; k < pts.size(); k++) {
            newPts.push_back(pts[k]);
            newPtsV.push_back(ptsV[k]);
        }
        KDTree tree(pts);
        for (unsigned int j = 0; j < pts.size(); j++) {
            vector<int> neighbors;
            tree.neighbors(pts, pts[j], num_neighbors, neighbor_radius, neighbors);
            Eigen::Vector3d v1 = moveTowardsCenter(neighbors, j);
            Eigen::Vector3d v2 = avoidCrowding(neighbors, j);
            Eigen::Vector3d v3 = matchVelocity(neighbors, j);
            Eigen::Vector3d v4 = stayInBounds(j);
            if (v1.norm() > v2.norm() && v1.norm() > v3.norm()) {
                v1strong++;
            } else if (v2.norm() > v1.norm() && v2.norm() > v3.norm()) {
                v2strong++;
            } else {
                v3strong++;
            }
            if (v1.norm() < 1) {
                v1small++;
            }
            v1avg += v1.norm();
            v2avg += v2.norm();
            v3avg += v3.norm();
            newPtsV[j] = (ptsV[j] + v1 + v2 + v3 + v4) * damping;
            newPts[j] = pts[j] + (newPtsV[j] * dt * 2);
            /*
            if (outOfBounds(newPts[j])) {
                newPtsV[j] = (-ptsV[j] + v1 + v2 + v3);
                newPts[j] = pts[j] + (newPtsV[j] * dt * damping);
            }
            */
            if (isnan(newPts[j][0])) {
                cout << "Something messed up!" << endl;
                cout << "pts[" << j << "]: " << pts[j][0] << " " << pts[j][1] << " " << pts[j][2] << endl;
                cout << "V1: " << v1[0] << " " << v1[1] << " " << v1[2] << endl;
                cout << "V2: " << v2[0] << " " << v2[1] << " " << v2[2] << endl;
                cout << "V3: " << v3[0] << " " << v3[1] << " " << v3[2] << endl;
            }

            fprintf(f, "[%f,%f,%f] [%f,%f,%f]\n", newPts[j][0], newPts[j][1], newPts[j][2], newPtsV[j][0], newPtsV[j][1], newPtsV[j][2]);
        }
        pts = newPts;
        ptsV = newPtsV;
        fprintf(f, "0\n");
        
        
    }

    v1avg /= 30000;
    v2avg /= 30000;
    v3avg /= 30000;
    cout << "Done!" << endl;
    cout << "Times v1 was strongest: " << v1strong << endl;
    cout << "Times v2 was strongest: " << v2strong << endl;
    cout << "Times v3 was strongest: " << v3strong << endl;
    cout << "Times v1 was less than 1: " << v1small << endl;
    cout << "Average strength of v1: " << v1avg << endl;
    cout << "Average strength of v2: " << v2avg << endl;
    cout << "Average strength of v3: " << v3avg << endl;
    fclose(f);
}

bool Boids::outOfBounds(Eigen::Vector3d pt) {
    if (pt[0] < -0.5 || pt[0] > 0.5)
        return true;
    else if (pt[1] < -0.25 || pt[1] > 0.25)
        return true;
    else if (pt[2] < -0.125 || pt[2] > 0.125)
        return true;
    return false;
}

Eigen::Vector3d Boids::moveTowardsCenter(vector<int> neighbors, unsigned int n) {
    Eigen::Vector3d center(0.0,0.0,0.0);
    for (unsigned int i = 0; i < neighbors.size(); i++) {
        if (neighbors[i] != n)
            center += pts[neighbors[i]];
    }
    if (neighbors.size() > 1) {
        center /= (neighbors.size() - 1);
    }
    //return (pts[n] - center) * centering;
    return (center - pts[n]) * centering;
}

Eigen::Vector3d Boids::avoidCrowding(vector<int> neighbors, unsigned int n) {
    Eigen::Vector3d c(0.0,0.0,0.0);
    Eigen::Vector3d pos = pts[n];
    for (unsigned int i = 0; i < neighbors.size(); i++) {
        if (neighbors[i] != n) {
            Eigen::Vector3d pos2 = pts[neighbors[i]];
            double nm = (pos - pos2).norm();
            //if (nm < collision) {
                //c -= (pos - pos2);
            //}
            //c -= (pos - pos2) * (collision / pow(nm, 2));
            c -= (pos2 - pos) * (collision / (pow(nm, 2) * 10));
            //c -= (pos2 - pos) * (collision);
        }
    }
    return c;
}

Eigen::Vector3d Boids::matchVelocity(vector<int> neighbors, unsigned int n) {
    Eigen::Vector3d v(0.0,0.0,0.0);
    for (unsigned int i = 0; i < neighbors.size(); i++) {
        if (neighbors[i] != n) {
            v += ptsV[neighbors[i]];
        }
    }
    if (neighbors.size() > 1) {
        v /= (neighbors.size() - 1);
    }
    //return (ptsV[n] - v) * velocity;
    return (v - ptsV[n]) * velocity;
}

Eigen::Vector3d Boids::stayInBounds(unsigned int n) {
    Eigen::Vector3d pos = pts[n];
    Eigen::Vector3d v(0.0,0.0,0.0);
    double force = 0.1;
    if (pos[0] < -0.5) {
        v[0] = force;
    } else if (pos[0] > 0.5) {
        v[0] = -force;
    }
    if (pos[1] < - 0.25) {
        v[1] = force;
    } else if (pos[1] > 0.25) {
        v[1] = -force;
    }
    if (pos[2] < -0.125) {
        v[2] = force;
    } else if (pos[2] > 0.125) {
        v[2] = -force;
    }
    return v;
}
void Boids::details() {
    cout << "Size: " << size << endl;
    cout << "Neighbor Radius: " << neighbor_radius << endl;
    cout << "Num Neighbors: " << num_neighbors << endl;
    cout << "Mass: " << mass << endl;
    cout << "Collision: " << collision << endl;
    cout << "Centering: " << centering << endl;
    cout << "Velocity: " << velocity << endl;
    cout << "Hunger: " << hunger << endl;
    cout << "Damping: " << damping << endl;
    cout << "Dt: " << dt << endl;
    cout << "Length: " << length << endl;
    
    //KDTree tree(pts);
    cout << "nboids: " << nboids << endl;
    for (int i = 0; i < 10; i++) {
        Eigen::Vector3d pos = pts[i];
        Eigen::Vector3d v = ptsV[i];
        KDTree tree(pts);
        vector<int> neighbors;
        tree.neighbors(pts, pos, 0, neighbor_radius, neighbors);
        cout << "[" << pos[0] << "," << pos[1] << "," << pos[2] << "] ";
        cout << "[" << v[0] << "," << v[1] << "," << v[2] << "] " << endl;
        cout << "Neighbors in range: " << neighbors.size() << endl;
    }
} 

int main(int argc, const char * argv[]) {
    if (argc < 2) {
        throw runtime_error("No input file given");
    }
    Boids boids(argv[1]);
    boids.draw(argv[2]);
    //boids.details();

    return 0;
}