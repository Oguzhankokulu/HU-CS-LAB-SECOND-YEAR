#include "GrayscaleImage.h"
#include <iostream>
#include <cstring>  // For memcpy
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <stdexcept>


// Constructor: load from a file
GrayscaleImage::GrayscaleImage(const char* filename) {

    // Image loading code using stbi
    int channels;
    unsigned char* image = stbi_load(filename, &width, &height, &channels, STBI_grey);

    if (image == nullptr) {
        std::cerr << "Error: Could not load image " << filename << std::endl;
        exit(1);
    }

    // Dynamically allocate memory for a 2D matrix based on the given dimensions.
    data = new int*[height];
    for (int i = 0; i < height; i++) {
        data[i] = new int[width];
    }
    // Fill the matrix with pixel values from the image
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            data[i][j] = image[i* width + j];
        }
    }

    // Free the dynamically allocated memory of stbi image
    stbi_image_free(image);
}

// Constructor: initialize from a pre-existing data matrix
GrayscaleImage::GrayscaleImage(int** inputData, int h, int w) {
    // Dynamically allocate memory for a 2D matrix based on the given dimensions.
    data = new int*[h];
    for (int i = 0; i < h; i++) {
        data[i] = new int[w];
    }
    // Fill the matrix with pixel values from the pre-existing data matrix.
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            data[i][j] = inputData[i][j];
        }
    }
}

// Constructor to create a blank image of given width and height
GrayscaleImage::GrayscaleImage(int w, int h) : width(w), height(h) {
    // Dynamically allocate memory for a 2D matrix based on the given dimensions.
    data = new int*[h];
    for (int i = 0; i < h; i++) {
        data[i] = new int[w];
    }
}

// Copy constructor
GrayscaleImage::GrayscaleImage(const GrayscaleImage& other) {
    height = other.height;
    width = other.width;
    // Dynamically allocate memory for a 2D matrix based on the object's dimensions.
    data = new int*[height];
    for (int i = 0; i < height; i++) {
        data[i] = new int[width];
    }
    // Fill the matrix with pixel values from the object.
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            data[i][j] = other.data[i][j];
        }
    }
}

// Destructor
GrayscaleImage::~GrayscaleImage() {
    // Delete each row
    for (int i = 0; i < height; ++i) {
        delete[] data[i];
    }
    // Delete the outer array
    delete[] data;
}

// Equality operator
bool GrayscaleImage::operator==(const GrayscaleImage& other) const {
    // If the dimensions are the same then look for the pixel values.
    if (height == other.height && width == other.width) {
        for (int i = 0; i < height; i++){
            for (int j = 0; j < width; j++){
                // If the pixel value is different then return false.
                if (data[i][j] != other.data[i][j]) {
                    return false;
                }
            }
        }
        // If the dimensions are the same and the pixels are the same return true.
        return true;
    }
    return false;
}

// Addition operator
GrayscaleImage GrayscaleImage::operator+(const GrayscaleImage& other) const {
    // Create a new image for the result
    GrayscaleImage result(width, height);
    
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            // If the addition of the pixels exceeds 255, clamp it at 255.
            if (data[i][j] + other.data[i][j] > 255){
                result.data[i][j] = 255;
            } else {
                result.data[i][j] = data[i][j] + other.data[i][j];
            }
        }
    }

    return result;
}

// Subtraction operator
GrayscaleImage GrayscaleImage::operator-(const GrayscaleImage& other) const {
    // Create a new image for the result
    GrayscaleImage result(width, height);
    
     for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            // If the subtraction of the pixels is below 0, clamp it at 0.
            if (data[i][j] - other.data[i][j] < 0){
                result.data[i][j] = 0;
            } else {
                result.data[i][j] = data[i][j] - other.data[i][j];
            }
        }
    }

    return result;
}

// Get a specific pixel value
int GrayscaleImage::get_pixel(int row, int col) const {
    return data[row][col];

}

// Set a specific pixel value
void GrayscaleImage::set_pixel(int row, int col, int value) {
    data[row][col] = value;
}

// Function to save the image to a PNG file
void GrayscaleImage::save_to_file(const char* filename) const {
    // Create a buffer to hold the image data in the format stb_image_write expects
    unsigned char* imageBuffer = new unsigned char[width * height];

    // Fill the buffer with pixel data (convert int to unsigned char)
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            imageBuffer[i * width + j] = static_cast<unsigned char>(data[i][j]);
        }
    }

    // Write the buffer to a PNG file
    if (!stbi_write_png(filename, width, height, 1, imageBuffer, width)) {
        std::cerr << "Error: Could not save image to file " << filename << std::endl;
    }

    // Clean up the allocated buffer
    delete[] imageBuffer;
}
