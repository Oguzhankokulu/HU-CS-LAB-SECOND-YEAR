#include "Map.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

Map::Map() {
    // TODO: Your code here
    // Initialize all distances to a value representing no direct connection
    // Initialize all provinces as unvisited

    for (int i = 0; i < MAX_SIZE; i++) {
        visited[i] = false;
        for (int j = 0; j < MAX_SIZE; j++) {
            distanceMatrix[i][j] = -1;
        }
    }

}

// Loads distance data from a file and fills the distanceMatrix
void Map::loadDistanceData(const std::string& filename) {
    // TODO: Your code here
    // Read each line in the CSV file
    // Read each cell separated by a comma
    // Convert cell to an integer and store in distanceMatrix

    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "File cannot be opened!\n";
        exit(-1);
    }

    std::string line;
    for (int i = 0; i < MAX_SIZE; i++){
        getline(file,line);
        std::stringstream ss(line);
        std::string distance;

        for (int j = 0; j <MAX_SIZE; j++) {
            getline(ss, distance, ',');
            int number = std::stoi(distance);

            distanceMatrix[i][j] = number;
        }

    }
}

// Checks if the distance between two provinces is within the allowed maxDistance
bool Map::isWithinRange(int provinceA, int provinceB, int maxDistance) const {
    // TODO: Your code here

    if (distanceMatrix[provinceA][provinceB] > maxDistance) {
        return false;
    }
    return true; 
}

// Marks a province as visited
void Map::markAsVisited(int province) {
    // TODO: Your code here
    visited[province] = true;
}

// Checks if a province has already been visited
bool Map::isVisited(int province) const {
    // TODO: Your code here
    if (visited[province] == true ) {
        return true;
    }
    return false;
}

// Resets all provinces to unvisited
void Map::resetVisited() {
    // TODO: Your code here
    for (int i = 0; i < MAX_SIZE; i++) {
        visited[i] = false;
    }
}

// Function to count the number of visited provinces
int Map::countVisitedProvinces() const {
    // TODO: Your code here
    int count = 0;

    for (int i = 0; i < MAX_SIZE; i++) {
        if (visited[i] == true) {
            count += 1;
        }
    }
    return count;
}

// Function to get the distance between two provinces
int Map::getDistance(int provinceA, int provinceB) const {
    // TODO: Your code here
    return distanceMatrix[provinceA][provinceB];
}