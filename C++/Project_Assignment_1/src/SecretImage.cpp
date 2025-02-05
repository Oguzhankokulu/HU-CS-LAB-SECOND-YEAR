#include "SecretImage.h"


// Constructor: split image into upper and lower triangular arrays
SecretImage::SecretImage(const GrayscaleImage& image) {
    width = image.get_width();
    height = image.get_height();

    int upperSize = (width*(width + 1))/2;
    int lowerSize = (width*(width - 1))/2;
    upper_triangular = new int[upperSize];
    lower_triangular = new int[lowerSize];

    int lowerCounter = 0;
    int upperCounter = 0;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < width; j++) {
            if (j < i) {
                lower_triangular[lowerCounter] = image.get_pixel(i,j);
                lowerCounter += 1;
            } else {
                upper_triangular[upperCounter] = image.get_pixel(i,j);
                upperCounter += 1;
            }
        }
    }
}

// Constructor: instantiate based on data read from file
SecretImage::SecretImage(int w, int h, int * upper, int * lower) {
    width = w;
    height = h;

    upper_triangular = upper;
    lower_triangular = lower;

}


// Destructor: free the arrays
SecretImage::~SecretImage() {
    delete[] upper_triangular;
    delete[] lower_triangular;
}

// Reconstructs and returns the full image from upper and lower triangular matrices.
GrayscaleImage SecretImage::reconstruct() const {
    GrayscaleImage image(width, height);

    int lowerCounter = 0;
    int upperCounter = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j < i) {
                image.set_pixel(i,j,lower_triangular[lowerCounter]);
                lowerCounter += 1;
            } else {
                image.set_pixel(i,j,upper_triangular[upperCounter]);
                upperCounter += 1;
            }
        }
    }

    return image;
}

// Save the filtered image back to the triangular arrays
void SecretImage::save_back(const GrayscaleImage& image) {
    int lowerCounter = 0;
    int upperCounter = 0;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < width; j++) {
            if (j < i) {
                lower_triangular[lowerCounter] = image.get_pixel(i,j);
                lowerCounter += 1;
            } else {
                upper_triangular[upperCounter] = image.get_pixel(i,j);
                upperCounter += 1;
            }
        }
    }
}

// Save the upper and lower triangular arrays to a file
void SecretImage::save_to_file(const std::string& filename) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "File cannot be opened!\n";
        exit(-1);
    }

    file << width << " " << height << std::endl;

    int upperSize = (width*(width + 1))/2;
    int lowerSize = (width*(width - 1))/2;

    for (int i = 0; i < upperSize; i++) {
        if (i == upperSize - 1) {
            file << upper_triangular[i] << std::endl;
        } else {
            file << upper_triangular[i] << " ";
        }
        
    }

    for (int i = 0; i < lowerSize; i++) {
        if (i == lowerSize - 1) {
            file << lower_triangular[i] << std::endl;
        } else {
            file << lower_triangular[i] << " ";
        }
    }

    file.close();
}

// Static function to load a SecretImage from a file
SecretImage SecretImage::load_from_file(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "File cannot be opened!\n";
        exit(-1);
    }

    int fileHeight = 0;
    int fileWidth = 0;
    file >> fileWidth >> fileHeight;

    int upperSize = (fileWidth*(fileWidth + 1))/2;
    int lowerSize = (fileWidth*(fileWidth - 1))/2;
    int* file_upper_triangular = new int[upperSize];
    int* file_lower_triangular = new int[lowerSize];

    for (int i = 0; i < upperSize; i++) {
        file >> file_upper_triangular[i];
    }

    for (int i = 0; i < lowerSize; i++) {
        file >> file_lower_triangular[i];
    }

    file.close();
    SecretImage secret_image(fileWidth, fileHeight, file_upper_triangular, file_lower_triangular);
    return secret_image;
}

// Returns a pointer to the upper triangular part of the secret image.
int * SecretImage::get_upper_triangular() const {
    return upper_triangular;
}

// Returns a pointer to the lower triangular part of the secret image.
int * SecretImage::get_lower_triangular() const {
    return lower_triangular;
}

// Returns the width of the secret image.
int SecretImage::get_width() const {
    return width;
}

// Returns the height of the secret image.
int SecretImage::get_height() const {
    return height;
}
