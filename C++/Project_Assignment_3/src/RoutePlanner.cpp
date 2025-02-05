#include "RoutePlanner.h"
#include <iostream>
#include <fstream>

// Array to help you out with name of the cities in order
const std::string cities[81] = { 
    "Adana", "Adiyaman", "Afyon", "Agri", "Amasya", "Ankara", "Antalya", "Artvin", "Aydin", "Balikesir", "Bilecik", 
    "Bingol", "Bitlis", "Bolu", "Burdur", "Bursa", "Canakkale", "Cankiri", "Corum", "Denizli", "Diyarbakir", "Edirne", 
    "Elazig", "Erzincan", "Erzurum", "Eskisehir", "Gaziantep", "Giresun", "Gumushane", "Hakkari", "Hatay", "Isparta", 
    "Mersin", "Istanbul", "Izmir", "Kars", "Kastamonu", "Kayseri", "Kirklareli", "Kirsehir", "Kocaeli", "Konya", "Kutahya", 
    "Malatya", "Manisa", "Kaharamanmaras", "Mardin", "Mugla", "Mus", "Nevsehir", "Nigde", "Ordu", "Rize", "Sakarya", 
    "Samsun", "Siirt", "Sinop", "Sivas", "Tekirdag", "Tokat", "Trabzon", "Tunceli", "Urfa", "Usak", "Van", "Yozgat", 
    "Zonguldak", "Aksaray", "Bayburt", "Karaman", "Kirikkale", "Batman", "Sirnak", "Bartin", "Ardahan", "Igdir", 
    "Yalova", "Karabuk", "Kilis", "Osmaniye", "Duzce" 
};

// Constructor to initialize and load constraints
RoutePlanner::RoutePlanner(const std::string& distance_data, const std::string& priority_data, const std::string& restricted_data, int maxDistance)
    : maxDistance(maxDistance), totalDistanceCovered(0), numPriorityProvinces(0), numWeatherRestrictedProvinces(0) {

    // TO DO:
    // Load map data from file
    // Mark all provinces as unvisited initially
    map.loadDistanceData(distance_data);
    map.resetVisited();

    totalDistanceCovered = 0;
    // Load priority provinces
    // Load restricted provinces
    loadPriorityProvinces(priority_data);
    loadWeatherRestrictedProvinces(restricted_data);
}

// Load priority provinces from txt file to an array of indices
void RoutePlanner::loadPriorityProvinces(const std::string& filename) {
    // TODO: Your code here
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "File cannot be opened!\n";
        exit(-1);
    }

    std::string line;
    int index = 0;
    while(getline(file, line)) {
        if (!line.empty() && index < MAX_PRIORITY_PROVINCES) {
            int start = line.find('(');
            int end = line.find(')');
            numPriorityProvinces += 1;
            std::string provinceStr = line.substr(start + 1, (end - 1) - start);
            priorityProvinces[index++] = std::stoi(provinceStr);
        }
    }
}

// Load weather-restricted provinces from txt file to an array of indices
void RoutePlanner::loadWeatherRestrictedProvinces(const std::string& filename) {
    // TODO: Your code here
    // TODO: Your code here
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "File cannot be opened!\n";
        exit(-1);
    }

    std::string line;
    int index = 0;
    while(getline(file, line)) {
        if (!line.empty() && index < MAX_WEATHER_RESTRICTED_PROVINCES) {
            int start = line.find('(');
            int end = line.find(')');
            numWeatherRestrictedProvinces += 1;
            std::string restrictProvince = line.substr(start + 1, (end - 1) - start);
            weatherRestrictedProvinces[index++] = std::stoi(restrictProvince);
        }
    }
}

// Checks if a province is a priority province
bool RoutePlanner::isPriorityProvince(int province) const {
    // TODO: Your code here
    bool isFound = false;
    for (int i = 0; i < numPriorityProvinces; i++) {
        if (province == priorityProvinces[i]) {
            isFound = true;
        }
    }
    return isFound;
}

// Checks if a province is weather-restricted
bool RoutePlanner::isWeatherRestricted(int province) const {
    // TODO: Your code here
    bool isFound = false;
    for (int i = 0; i < numWeatherRestrictedProvinces; i++) {
        if (province == weatherRestrictedProvinces[i]) {
            isFound = true;
        }
    }
    return isFound;
}

// Begins the route exploration from the starting point
void RoutePlanner::exploreRoute(int startingCity) {
    // TODO: Your code here
    int currentProvince = startingCity;
    exploreFromProvince(currentProvince);
    while(!isExplorationComplete()) {
        if (!queue.isEmpty()){
            currentProvince = queue.dequeue();
        } else { // If the queue is empty backtrack
            backtrack();
            continue;
        }

        if (map.distanceMatrix[stack.peek()][currentProvince] <= 250 && map.distanceMatrix[stack.peek()][currentProvince] != 0) {
            totalDistanceCovered += map.distanceMatrix[stack.peek()][currentProvince];
            exploreFromProvince(currentProvince);
            if (totalDistanceCovered > longestDistanceCovered) {
                longestDistanceCovered = totalDistanceCovered;
                longestRoute = route;
            }
        }
    }

    route = longestRoute;
    totalDistanceCovered = longestDistanceCovered;
    displayResults();
}

// Helper function to explore from a specific province
void RoutePlanner::exploreFromProvince(int province) {
    // TODO: Your code here 
    map.markAsVisited(province);
    route.push_back(province);
    stack.push(province);
    enqueueNeighbors(province);

}

void RoutePlanner::enqueueNeighbors(int province) {
    // TO DO: Enqueue priority & non-priority neighbors to the queue according to given constraints
    for (int i = 0; i < 81; i++) {
        int distance = map.distanceMatrix[province][i]; // Distance between province and neighbor
        if (distance <= maxDistance && !map.isVisited(i) && !queue.isInQueue(i)) { // If its below max distance and not itself and not visited
            if (isWeatherRestricted(i)) {
                std::cout << "Province " << cities[i] << " is weather-restricted. Skipping\n";
                continue;
            }
            bool isPrior = isPriorityProvince(i);

            if (isPrior) { // Check if its priority
                queue.enqueuePriority(i);
            } else {
                queue.enqueue(i);
            }
        }
    }
}

void RoutePlanner::backtrack() {
    // If you reach a dead-end province 
    // TODO: Your code here
    int removed = stack.pop();
    route.pop_back();
    
    if (!stack.isEmpty()) {
        totalDistanceCovered -= map.distanceMatrix[removed][stack.peek()];
        enqueueNeighbors(stack.peek());
    }
}

bool RoutePlanner::isExplorationComplete() const {
    // TODO: Your code here
    return stack.isEmpty() && queue.isEmpty();
}

void RoutePlanner::displayResults() const {
    // TODO: Your code here
    // Display "Journey Completed!" message
    // Display the total number of provinces visited
    // Display the total distance covered
    // Display the route in the order visited
    // Priority Province Summary

    std::cout << "----------------------------\n";
    std::cout << "Journey Completed!\n";
    std::cout << "----------------------------\n";
    std::cout << "Total Number of Provinces Visited: " << route.size() << "\n";
    std::cout << "Total Distance Covered: " << totalDistanceCovered << " km\n";
    std::cout << "Route Taken:\n";
    std::cout << cities[route[0]];
    for (int i = 1; i < route.size(); i++) {
        std::cout << " -> " << cities[route[i]];
    }
    std::cout << "\n";
    std::cout << "Priority Provinces Status:\n";
    int count = 0;
    for (int i = 0; i < numPriorityProvinces; i++) {
        std::string visitStr = "";
        if (map.visited[priorityProvinces[i]]) {
            count++;
            visitStr = "Visited";
        } else {
            visitStr = "Not Visited";
        }
        std::cout << "- " << cities[priorityProvinces[i]] << " (" << visitStr << ")\n";
    }
    std::cout << "\n";
    std::cout << "Total Priority Provinces Visited: " << count << " out of " << numPriorityProvinces << "\n";
    if (count == numPriorityProvinces) {
        std::cout << "Success: All priority provinces were visited.";
    } else {
        std::cout << "Warning: Not all priority provinces were visited.";
    }
}