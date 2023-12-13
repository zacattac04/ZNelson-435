#include <Eigen/Dense>
#include <iostream>
#include <cmath>
using namespace Eigen;
using namespace std;



int main() {


    MatrixXd m(50,30);
    int rows = m.rows();
    int cols = m.cols();
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            m(i,j) = -i+j;
        }
    }
    m = m.array().abs();
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            //cout << m(i,j) << " ";
        }
        //cout << endl;
    }

    //MatrixXd<Vector3d, 30, 50> image;
    cout << "Done" << endl;
    return 0;
}