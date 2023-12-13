#define cimg_display 0
#define cimg_use_jpg
#include "CImg.h"
#include <Eigen/Dense>
#include <iostream>
#include <cmath>

using namespace cimg_library;
using namespace Eigen;
using namespace std;


/*
// This is a simple 2-d convolution function
// It was originally meant for using the sobel operators, which I didn't end up doing
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
*/
/*
// This was my attempt at using the sobel operators to find the energy map
// While it seemingly worked, it took several minutes to calculate the energy once
// This would have taken too long for practical use
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

// This creates an energy map using the simple energy function in the paper
// That being, the energy of the image is the absolute value of the partial derivative of x, plus the absolute value of the partial derivative of y.
// It uses the central difference in most cases, and forward difference and backward difference on the edges
MatrixXd computeSimpleMap(MatrixXd image) {
    int height = image.rows();
    int width = image.cols();
    MatrixXd edgesX(height, width);
    MatrixXd edgesY(height, width);
    //cout << "Convolving..." << endl;
    for (int i = 0; i < (height / 10); i++) {
        //cout << "-";
    }
    //cout << endl;
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
            //cout << "*";
        }
    }


    MatrixXd energyMap(height, width);
    energyMap = edgesX.array().abs() + edgesY.array().abs();
    return energyMap;
}

// This returns a "greyscale" image by taking the average of all three channels
// In practice, this isn't a true greyscale, as we are using LAB, not RGB
// A true greyscale would simple have been to pass only the first channel (luminance)
// However, this would mean that two pixels with the same luminance but different colors would be equal
// I felt that it was best in practice to take the average, as this takes into account all of the data
// I also didn't account for the fact that the channels are bounded differently (L from [0,100], A and B from [-128, 128])
// It seems to work fine though, so I'm leaving it as is
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

// Given an energy map, this will find the optimal vertical seam
// It returns a vector of indices representing which pixel in each column to not include
vector<int> findSeam(MatrixXd energyMap) {
    int rows = energyMap.rows();
    int cols = energyMap.cols();

    MatrixXd cost(rows, cols);
    cost.row(0) = energyMap.row(0);

    for (int i = 1; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (j == 0) {
                cost(i, j) = energyMap(i, j) + min(cost(i-1, j), cost(i-1, j+1));
            } else if (j == cols - 1) {
                cost(i, j) = energyMap(i, j) + min(cost(i-1, j-1), cost(i-1, j));
            } else {
                cost(i, j) = energyMap(i, j) + min({cost(i-1, j-1), cost(i-1,j), cost(i-1, j+1)});
            }
        }
    }

    int minIndex = 0;
    for (int j = 1; j < cols; j++) {
        if (cost(rows-1, j) < cost(rows-1, minIndex)) {
            minIndex = j;
        }
    }

    vector<int> seam(rows);
    seam[rows-1] = minIndex;
    for(int i = rows - 2; i >= 0; i--) {
        if (minIndex == 0) {
            minIndex = cost(i, minIndex) < cost(i, minIndex + 1) ? minIndex : minIndex + 1;
        } else if (minIndex == cols - 1) {
            minIndex = cost(i, minIndex - 1) < cost(i, minIndex) ? minIndex - 1 : minIndex;
        } else {
            int minNeighbor = minIndex - 1;
            for (int j = minIndex - 1; j <= minIndex + 1; j++) {
                if (cost(i, j) < cost(i, minNeighbor)) {
                    minNeighbor = j;
                }
            }
            minIndex = minNeighbor;
        }

        seam[i] = minIndex;
    }

    return seam;
}

// This is an alternative way to call findSeam
// It is given the image vector, height, and width
vector<int> findSeam(Vector3d *image, int height, int width) {
    MatrixXd greyscale = greyscaleImage(image, height, width);
    MatrixXd energyMap = computeSimpleMap(greyscale);
    return findSeam(energyMap);
}

// This finds the max energy in the energyMap
// This is for the purpose of printing the energy image
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

// This is a simple function that transposes the image vector
Vector3d *transpose(Vector3d *image, int height, int width) {
    Vector3d *transposedImage = new Vector3d[height*width];

    for(int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            transposedImage[j*width+i] = image[i*height+j];
        }
    }
    return transposedImage;
}

// This is the function that resizes the image
// It always resizes the width first, and then the height
// Since the findSeam function only finds vertical seams, we transpose the image before resizing the height
// Effectively, we are just resizing the width of a transposed image
// Afterwards, we transpose the image again to return it to the original orientation
// At the end we return the resized image
Vector3d *resize(Vector3d *image, int height, int width, int newHeight, int newWidth) {
    while (newWidth < width) {
        MatrixXd greyscale = greyscaleImage(image, height, width);
        MatrixXd energyMap = computeSimpleMap(greyscale);
        vector<int> seam = findSeam(energyMap);
        width--;
        Vector3d *newImage = new Vector3d[width*height];
        for (int i = 0; i < height; i++) {
            for(int j = 0; j < seam[i]; j++) {
                newImage[j*height+i] = image[j*height+i];
            }
            for (int j = seam[i]; j < width; j++) {
                newImage[j*height+i] = image[(j+1)*height+i];
            }
        }
        image = newImage;
    }

    image = transpose(image, height, width);
    int temp = height;
    height = width;
    width = temp;
    temp = newHeight;
    newHeight = newWidth;
    newWidth = temp;
    while (newWidth < width) {
        MatrixXd greyscale = greyscaleImage(image, height, width);
        MatrixXd energyMap = computeSimpleMap(greyscale);
        vector<int> seam = findSeam(energyMap);
        width--;
        Vector3d *newImage = new Vector3d[width*height];
        for (int i = 0; i < height; i++) {
            for(int j = 0; j < seam[i]; j++) {
                newImage[j*height+i] = image[j*height+i];
            }
            for (int j = seam[i]; j < width; j++) {
                newImage[j*height+i] = image[(j+1)*height+i];
            }
        }
        image = newImage;
    }
    image = transpose(image, height, width);

    return image;
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

    MatrixXd greyscale = greyscaleImage(image, input.height(), input.width());
    cout << "greyscale rows: " << greyscale.rows() << endl;
    cout << "greyscale cols: " << greyscale.cols() << endl;
    MatrixXd energyMap = computeSimpleMap(greyscale);
    cout << "Creating energy image...";
    CImg<double> energy(input.width(), input.height(), input.depth(), input.spectrum(), 0);
    cout << "Done." << endl;
    cout << "Finding max value...";
    double maxE = maxEnergy(energyMap);
    cout << "Done." << endl;
    vector<int> seam = findSeam(energyMap);
    for (int i=0; i<input.width(); i++) {
        for (int j=0; j<input.height(); j++) {
            energy(i, j, 0) = pow(energyMap(j, i)/maxE, 1.0/3.0) * 100;
            energy(i, j, 1) = 0;
            energy(i, j, 2) = 0;
        }
    }
    Vector3d *transposedImage = transpose(image, input.height(), input.width());
    vector<int> seamH = findSeam(transposedImage, input.width(), input.height());
    for(int i = 0; i < input.height(); i++) {
        energy(seam[i], i, 0) = 100;
        energy(seam[i], i, 1) = 128;
    }
    for(int i = 0; i < input.width(); i++) {
        energy(i, seamH[i], 0) = 100;
        energy(i, seamH[i], 2) = 128;
    }

    int newHeight, newWidth;
    if (argc <= 3) {
        newHeight = input.height();
        newWidth = input.width();
    } else {
        newWidth = atoi(argv[3]);
        newHeight = atoi(argv[4]);
        if (newWidth > input.width()) {
            cout << "Error: Width given is greater than image width. Reverting to image width." << endl;
            newWidth = input.width();
        }
        if (newHeight > input.height()) {
            cout << "Error: Height given is greater than image Height. Reverting to image Height." << endl;
            newHeight = input.height();
        }
    }

    cout << "Resizing...";
    image = resize(image, input.height(), input.width(), newHeight, newWidth);
    cout << "Done." << endl;
    CImg<double> output(newWidth, newHeight, input.depth(), input.spectrum(), 0);

    for (int i=0; i<output.height();i++) {
        for (int j = 0; j < output.width(); j++) {
            output(j, i, 0) = image[j*output.height()+i][0];
            output(j, i, 1) = image[j*output.height()+i][1];
            output(j, i, 2) = image[j*output.height()+i][2];
        }
    }

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
