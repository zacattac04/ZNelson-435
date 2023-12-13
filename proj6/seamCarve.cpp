#define cimg_display 0
#define cimg_use_jpg
#include "CImg.h"
#include <Eigen/Dense>
#include <iostream>
#include <cmath>
using namespace cimg_library;
using namespace Eigen;
using namespace std;

double convolve(MatrixXd image, MatrixXd filter, int x, int y) {
    int cols = image.cols();
    int rows = image.rows();
    
    if (x > 0 && x < (rows -1) && y > 0 && y < (cols -1)) {
        MatrixXd block = image.block(x-1, y-1, 3, 3);
        double sum = (block.array() * filter.array()).sum();
        return sum;
    }

    double sum = 0.0;
    for (int i = 0; i < 3; i++) {
        //cout << cols << ", " << rows << "\t\t";
        for (int j = 0; j < 3; j++) {
            int checkX = x + i - 1;
            int checkY = y + j - 1;
            //cout << checkX << ", " << checkY << "\t";
            if (checkX >= 0 && checkX < rows && checkY >= 0 && checkY < cols) {
                sum += (image(checkX, checkY) * (filter(i, j)));
            }
        }
        //cout << endl;
    }
    return sum;
}

/*
MatrixXd computeSobelMap(MatrixXd greyscale) {
    int height = greyscale.rows();
    int width = greyscale.cols();
    cout << "Height: " << height << endl;
    cout << "Width: " << width << endl;
    Matrix3d sobelX;
    sobelX << 1, 0, -1,
              2, 0, -2,
              1, 0, -1;
    
    Matrix3d sobelY;
    sobelY << 1, 2, 1,
              0, 0, 0,
              -1, -2, -1;

    MatrixXd edgesX(height, width);
    MatrixXd edgesY(height, width);
    cout << "Convolving..." << endl;
    for (int i = 0; i < (height / 10); i++) {
        cout << "-";
    }
    cout << endl;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            //cout << i << "\t" << j << endl;
            double x = convolve(greyscale, sobelX, i, j);
            double y = convolve(greyscale, sobelY, i, j);
            edgesX(i, j) = x;
            edgesY(i, j) = y;
            
        }
        if (i % 10 == 0) {
            cout << "*";
        }
    }


    MatrixXd energyMap(height, width);
    energyMap = edgesX.array().square() + edgesY.array().square();
    energyMap = energyMap.array().sqrt();
    double minE = 1000;
    double maxE = -1000;
    double avgE = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double e = energyMap(i, j);

            avgE += e;
        }
    }

    return energyMap;
}
*/

MatrixXd computeSimpleMap(MatrixXd image) {
    int height = image.rows();
    int width = image.cols();
    MatrixXd edgesX(height, width);
    MatrixXd edgesY(height, width);
    cout << "Convolving..." << endl;
    for (int i = 0; i < (height / 10); i++) {
        cout << "-";
    }
    cout << endl;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i > 0 && i < (height-1)) {
                double lowI = image(i-1, j);
                double highI = image(i+1, j);
                double diff = (highI - lowI) / 2;
                edgesX(i, j) = diff;
            } else if (i == 0) {
                double highI = image(i+1, j);
                double diff = highI - image(i,j);
                edgesX(i,j) = diff;
            } else if (i == (height-1)) {
                double lowI = image(i-1,j);
                double diff = image(i,j) - lowI;
                edgesX(i, j) = diff;
            }

            if (j > 0 && j < (width-1)) {
                double lowJ = image(i, j-1);
                double highJ = image(i, j+1);
                double diff = (highJ - lowJ) / 2;
                edgesY(i, j) = diff;
            } else if (j == 0) {
                double highJ = image(i, j+1);
                double diff = highJ - image(i,j);
                edgesY(i,j) = diff;
            } else if (j == (width-1)) {
                double lowJ = image(i,j-1);
                double diff = image(i,j) - lowJ;
                edgesY(i, j) = diff;
            }
        }
        if (i % 10 == 0) {
            cout << "*";
        }
    }


    MatrixXd energyMap(height, width);
    energyMap = edgesX.array().abs() + edgesY.array().abs();
    return energyMap;
}

MatrixXd greyscaleImage(Vector3d image[], int height, int width) {
    MatrixXd greyscale(height, width);
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            double r = image[i*height+j][0];
            double b = image[i*height+j][1];
            double g = image[i*height+j][2];
            double grey = floor((r + b + g) / 3);
            greyscale(j, i) = grey;
        }
    }

    return greyscale;
}

double maxEnergy(MatrixXd energyMap) {
    int height = energyMap.rows();
    int width = energyMap.cols();
    double max = -1000;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (energyMap(i,j) > max) {
                max = energyMap(i,j);
            }
        }
    }
    return max;
}


int main(int argc, char *argv[]) {
    CImg<double> input(argv[1]);
    CImg<double> lab = input.RGBtoLab();
    Vector3d *image = new Vector3d[input.width()*input.height()];
    for (int i=0; i<input.width(); i++) {
        for (int j=0; j<input.height(); j++) {
            image[i*input.height()+j][0] = lab(i, j, 0);
            image[i*input.height()+j][1] = lab(i, j, 1);
            image[i*input.height()+j][2] = lab(i, j, 2);
        }
    }
    int newHeight, newWidth;
    //std::cout << "Num Args: " << argc << std::endl;
    if (argc <= 3) {
        newHeight = input.height();
        newWidth = input.width();
    } else {
        newWidth = atoi(argv[3]);
        newHeight = atoi(argv[4]);
    }
    CImg<double> output(newWidth, newHeight, input.depth(), input.spectrum(), 0);
    for (int i=0; i<output.width(); i++) {
        for (int j=0; j<output.height(); j++) {
            output(i, j, 0) = image[i*output.height()+j][0];
            output(i, j, 1) = image[i*output.height()+j][1];
            output(i, j, 2) = image[i*output.height()+j][2];
        }
    }
    MatrixXd greyscale = greyscaleImage(image, input.height(), input.width());
    cout << "greyscale rows: " << greyscale.rows() << endl;
    cout << "greyscale cols: " << greyscale.cols() << endl;
    MatrixXd energyMap = computeSimpleMap(greyscale);
    CImg<double> grey(input.width(), input.height(), input.depth(), input.spectrum(), 0);
    cout << "Creating energy image..." << endl;
    CImg<double> energy(input.width(), input.height(), input.depth(), input.spectrum(), 0);
    cout << "Done." << endl;
    cout << "Finding max value..." << endl;
    double maxE = maxEnergy(energyMap);
    cout << "Done." << endl;
    for (int i=0; i<input.width(); i++) {
        for (int j=0; j<input.height(); j++) {
            grey(i, j, 0) = lab(i, j, 0);
            grey(i, j, 1) = 0;
            grey(i, j, 2) = 0;
            energy(i, j, 0) = pow(energyMap(j, i)/maxE, 1.0/3.0) * 100;
            energy(i, j, 1) = 0;
            energy(i, j, 2) = 0;
        }
    }

    //CImg<double> greyOut = grey.LabtoRGB();
    //greyOut.save_jpeg("greyscale.jpg");
    CImg<double> energyOut = energy.LabtoRGB();
    energyOut.save_jpeg("energy.jpg");
    CImg<double> rgb = output.LabtoRGB();
    if (strstr(argv[2], "png"))
	    rgb.save_png(argv[2]);
    else if (strstr(argv[2], "jpg"))
	    rgb.save_jpeg(argv[2]);

    delete [] image;
    return 0;
}
