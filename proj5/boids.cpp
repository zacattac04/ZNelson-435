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

// This function will move the boids based on the constraints
// and write the positions and velocity to the file
// It is currently set up to only deal with boids, and not food objects
void Boids::draw(const char * & fname) {
    FILE * f = fopen(fname, "wb");
    int steps = (int)ceil(length / dt);
    fprintf(f, "%d\n", steps);
    cout << "Drawing animation: Steps out of " << steps << endl;
    for (int i = 0; i < steps; i++) {
        if (i % 50 == 0) {
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
            newPtsV[j] = (ptsV[j] + v1 + v2 + v3 + v4) * damping;
            newPts[j] = pts[j] + (newPtsV[j] * dt * 2);
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
    fclose(f);
}

// This creates a velocity vector
// That moves the boid to the average heading of the neighboring boids
Eigen::Vector3d Boids::moveTowardsCenter(vector<int> neighbors, unsigned int n) {
    Eigen::Vector3d center(0.0,0.0,0.0);
    for (unsigned int i = 0; i < neighbors.size(); i++) {
        if ((unsigned int)neighbors[i] != n)
            center += pts[neighbors[i]];
    }
    if (neighbors.size() > 1) {
        center /= (neighbors.size() - 1);
    }
    //return (pts[n] - center) * centering;
    return (center - pts[n]) * centering;
}

// This creates a velocity vector
// that moves the boid away from other boids that are too close
Eigen::Vector3d Boids::avoidCrowding(vector<int> neighbors, unsigned int n) {
    Eigen::Vector3d c(0.0,0.0,0.0);
    Eigen::Vector3d pos = pts[n];
    for (unsigned int i = 0; i < neighbors.size(); i++) {
        if ((unsigned int)neighbors[i] != n) {
            Eigen::Vector3d pos2 = pts[neighbors[i]];
            double nm = (pos - pos2).norm();
            c -= (pos2 - pos) * (collision / (pow(nm, 2) * 10)); // I reduced the force by 10, as I found this vector to overpower the others
        }
    }
    return c;
}

// This creates a velocity vector
// That matches the heading/velocity of the boid with neighboring boids
Eigen::Vector3d Boids::matchVelocity(vector<int> neighbors, unsigned int n) {
    Eigen::Vector3d v(0.0,0.0,0.0);
    for (unsigned int i = 0; i < neighbors.size(); i++) {
        if ((unsigned int)neighbors[i] != n) {
            v += ptsV[neighbors[i]];
        }
    }
    if (neighbors.size() > 1) {
        v /= (neighbors.size() - 1);
    }
    return (v - ptsV[n]) * velocity;
}

// This creates a velocity vector
// That pushes the boid away from the bounds of the scene
// This does not create a rigid "box".
// It produces a force proportional to the distance out of bounds the boid is.
// When using the Godot viewer, some of the boids will temporarily disappear.
// I felt it best to keep as is, as it made a more natural simulation than rigidly bouncing off the box
Eigen::Vector3d Boids::stayInBounds(unsigned int n) {
    Eigen::Vector3d pos = pts[n];
    Eigen::Vector3d v(0.0,0.0,0.0);
    if (pos[0] < -0.5) {
        //v[0] = force;
        v[0] = pow(pos[0] + 0.5, 2);
    } else if (pos[0] > 0.5) {
        //v[0] = -force;
        v[0] = -1 * pow(pos[0] - 0.5, 2);
    }
    if (pos[1] < - 0.25) {
        //v[1] = force;
        v[1] = pow(pos[1] + 0.25, 2);
    } else if (pos[1] > 0.25) {
        //v[1] = -force;
        v[1] = -1 * pow(pos[1] - 0.25, 2);
    }
    if (pos[2] < -0.125) {
        //v[2] = force;
        v[2] = pow(pos[2] + 0.125, 2);
    } else if (pos[2] > 0.125) {
        //v[2] = -force;
        v[2] = -1 * pow(pos[2] - 0.125, 2);
    }
    return v;
}

// This prints basic details as read from the sample.in file
// This code will not activate from the code as is
// I leave it here for posteriety, but it is not important
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
    
    KDTree tree(pts);
    cout << "nboids: " << nboids << endl;
    for (int i = 0; i < 10; i++) {
        Eigen::Vector3d pos = pts[i];
        Eigen::Vector3d v = ptsV[i];
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