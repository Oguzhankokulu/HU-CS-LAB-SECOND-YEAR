#include "Filter.h"
#include <algorithm>
#include <cmath>
#include <vector>
#include <numeric>
#include <math.h>
#include <iostream>

// Mean Filter
void Filter::apply_mean_filter(GrayscaleImage& image, int kernelSize) {
    // TODO: Your code goes here.
    // 1. Copy the original image for reference.
    // 2. For each pixel, calculate the mean value of its neighbors using a kernel.
    // 3. Update each pixel with the computed mean.
    // Copy of the original image for reference.
    GrayscaleImage* copyImage = new GrayscaleImage(image);

    // 2. For each pixel, calculate the mean value of its neighbors using a kernel.
    int resultNumber = 0;
    int height = copyImage->get_height();
    int width = copyImage->get_width();
    // Distance to edge of the kernel from the center.
    int distance = kernelSize/2;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            resultNumber = 0;
            /* 
            To get the index of the kernel pixels (Kernell size must be odd.).
            For example for a 5 kernell size, it queries starting from
            [i-2][j-2] to [i+2][j+2]            
            */
            for (int m = -distance; m <= distance; m++){
                for (int n = -distance; n <= distance; n++){
                    if (i+m < 0 || i+m >= height || j+n < 0 || j+n >= width) {
                        continue;
                    } else {
                        resultNumber += copyImage->get_pixel(i+m,j+n);
                    }
                }
            }
            resultNumber = resultNumber / (kernelSize*kernelSize);
            image.set_pixel(i,j,resultNumber);
        }
    }
    
    delete copyImage;
}

// Gaussian Smoothing Filter
void Filter::apply_gaussian_smoothing(GrayscaleImage& image, int kernelSize, double sigma) {
    GrayscaleImage* copyImage = new GrayscaleImage(image);
    std::vector<std::vector<double>> gaussianDistribution(kernelSize, std::vector<double>(kernelSize));

    // Initialize the gaussian kernel by calculating each index.
    int distance = kernelSize/2;
    double sum = 0;
    double gaussValue = 0;
    for (int i = -distance; i <= distance; i++) {
        for (int j = -distance; j <= distance; j++) {
            // Gaussian distribution calculation.
            gaussValue = (exp(-((double)((i*i) + (j * j))/(2.0*sigma*sigma))))/(2.0*M_PI*sigma*sigma);
            sum += gaussValue;
            gaussianDistribution[i+distance][j+distance] = gaussValue;
        }
    }

    //Normalize the kernel.
    for (int i = 0; i < kernelSize; i++) {
        for (int j = 0; j < kernelSize; j++) {
            gaussianDistribution[i][j] /= sum;
        }
    }

    double resultNumber = 0;
    int height = copyImage->get_height();
    int width = copyImage->get_width();
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            resultNumber = 0;
            /* 
            To get the index of the kernel pixels (Kernell size must be odd.).
            For example for a 5 kernell size, it queries starting from
            [i-2][j-2] to [i+2][j+2]            
            */
            for (int m = -distance; m <= distance; m++){
                for (int n = -distance; n <= distance; n++){
                    if (i+m < 0 || i+m >= height || j+n < 0 || j+n >= width) {
                        continue;
                    } else {
                        // Add the value calculated according to gaussian kernel.
                        resultNumber += copyImage->get_pixel(i+m,j+n)*gaussianDistribution[m+distance][n+distance];
                    }
                }
            }
            image.set_pixel(i,j,resultNumber);
        }
    }

    delete copyImage;
}

// Unsharp Masking Filter
void Filter::apply_unsharp_mask(GrayscaleImage& image, int kernelSize, double amount) {

    GrayscaleImage* copyImage = new GrayscaleImage(image);

    apply_gaussian_smoothing(*copyImage, kernelSize);

    int sharpValue = 0;
    for (int i = 0; i < image.get_height(); i++) {
        for (int j = 0; j < image.get_width(); j++){
            sharpValue = image.get_pixel(i,j) + (amount * (image.get_pixel(i,j) - copyImage->get_pixel(i,j)));
            if (sharpValue < 0) {
                image.set_pixel(i,j,0);
            } else if (sharpValue > 255) {
                image.set_pixel(i,j,255);
            } else {
                image.set_pixel(i,j,sharpValue);
            }
        }
    }

    delete copyImage;
}
