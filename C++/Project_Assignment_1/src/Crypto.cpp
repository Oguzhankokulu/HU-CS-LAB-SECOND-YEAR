#include "Crypto.h"
#include "GrayscaleImage.h"
#include <cmath>

// Extract the least significant bits (LSBs) from SecretImage, calculating x, y based on message length
std::vector<int> Crypto::extract_LSBits(SecretImage& secret_image, int message_length) {
    std::vector<int> LSB_array;
    // TODO: Your code goes here.

    // 1. Reconstruct the SecretImage to a GrayscaleImage.
    // 2. Calculate the image dimensions.
    // 3. Determine the total bits required based on message length.
    // 4. Ensure the image has enough pixels; if not, throw an error.
    // 5. Calculate the starting pixel from the message_length knowing that  
    //    the last LSB to extract is in the last pixel of the image.
    // 6. Extract LSBs from the image pixels and return the result.

    GrayscaleImage image = secret_image.reconstruct();

    int total_bits = message_length * 7;
    int image_size = image.get_height() * image.get_width();

    if (total_bits > image_size) {
        std::cerr << "Message is too long to embed!" << std::endl;
        exit(-1);
    }

    int LSB = 0;
    int row = 0;
    int column = 0;
    for (int i = (image_size-total_bits); i < image_size; i++) {
        row = i/(image.get_width());
        column = i%(image.get_width());
        LSB = image.get_pixel(row,column)%2;
        // write a function to convert decimal numbers to binary.
        LSB_array.push_back(LSB);
    }

    return LSB_array;
}


// Decrypt message by converting LSB array into ASCII characters
std::string Crypto::decrypt_message(const std::vector<int>& LSB_array) {
    std::string message;
    // TODO: Your code goes here.

    // 1. Verify that the LSB array size is a multiple of 7, else throw an erro r.
    // 2. Convert each group of 7 bits into an ASCII character.
    // 3. Collect the characters to form the decrypted message.
    // 4. Return the resulting message.

    if (LSB_array.size() % 7 != 0) {
        std::cerr << "Wrong LSB array size!\n";
        exit(-1);
    }

    char charVal;
    for (int i = 0; i < LSB_array.size(); i += 7) {
        int value = 0;
        for (int j = 0; j < 7; j++) {
            value += LSB_array[i+j] * std::pow(2, 6 - j);
        }
        charVal = value;
        message += charVal;
    }

    return message;
}

// Encrypt message by converting ASCII characters into LSBs
std::vector<int> Crypto::encrypt_message(const std::string& message) {
    std::vector<int> LSB_array;
    // TODO: Your code goes here.

    // 1. Convert each character of the message into a 7-bit binary representation.
    //    You can use std::bitset.
    // 2. Collect the bits into the LSB array.
    // 3. Return the array of bits.

    for (int i = 0; i < message.size(); i++) {
        std::bitset<7> binary(message[i]);
        for (int j = (binary.size() - 1); j >= 0; j--) {
            LSB_array.push_back(binary[j]);
        }
    }

    return LSB_array;
}

// Embed LSB array into GrayscaleImage starting from the last bit of the image
SecretImage Crypto::embed_LSBits(GrayscaleImage& image, const std::vector<int>& LSB_array) {
    // TODO: Your code goes here.
    // 1. Ensure the image has enough pixels to store the LSB array, else throw an error.
    // 2. Find the starting pixel based on the message length knowing that  
    //    the last LSB to embed should end up in the last pixel of the image.
    // 3. Iterate over the image pixels, embedding LSBs from the array.
    // 4. Return a SecretImage object constructed from the given GrayscaleImage 
    //    with the embedded message.
    int imageSize = image.get_height() * image.get_width();
    
    if (imageSize < LSB_array.size()) {
        std::cerr << "Image is small to embed LSBits.\n";
        exit(-1);
    }

    int startingRow = (imageSize - LSB_array.size()) / image.get_width();
    int startingColumn = (imageSize - LSB_array.size()) % image.get_width();
    int counter = 0;
    int pixelVal = 0;
    for (int i = startingRow; i < image.get_height(); i++) {
        for (int j = startingColumn; j < image.get_width(); j++) {
            std::bitset<8> binary(image.get_pixel(i,j));
            binary[0] = LSB_array[counter];
            counter++;
            pixelVal = static_cast<int>(binary.to_ulong());
            image.set_pixel(i,j,pixelVal);
        }
        startingColumn = 0;
    }

    SecretImage secret_image(image);

    return secret_image;
}
