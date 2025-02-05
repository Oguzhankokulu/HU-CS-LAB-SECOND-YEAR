#include "RealmShapers.h"
#include <cmath>
#include <algorithm>

ShaperTree::ShaperTree()
{
}

ShaperTree::~ShaperTree()
{
    // TODO: Free any dynamically allocated memory if necessary
    for (int i = 0; i < realmShapers.size(); i++) {
        delete realmShapers[i];
    }

    realmShapers.clear();
}

void ShaperTree::initializeTree(std::vector<RealmShaper *> shapers)
{
    // TODO: Insert innitial shapers to the tree
    realmShapers = shapers;
}

int ShaperTree::getSize()
{
    // TODO: Return number of shapers in the tree
    return realmShapers.size();
}

std::vector<RealmShaper *> ShaperTree::getTree()
{
    return realmShapers;
}

bool ShaperTree::isValidIndex(int index)
{
    bool isValid = false;

    // TODO: Check if the index is valin in the tree
    if (0 <= index && index < realmShapers.size()) {
        isValid = true;
    }

    return isValid;
}

void ShaperTree::insert(RealmShaper *shaper)
{
    // TODO: Insert shaper to the tree
    realmShapers.push_back(shaper);
}

int ShaperTree::remove(RealmShaper *shaper)
{
    // TODO: Remove the player from tree if it exists
    // Make sure tree protects its form (complate binary tree) after deletion of a node
    // return index if found and removed
    // else
    for (int i = 0; i < realmShapers.size(); i++) {
        if (realmShapers[i] == shaper) {
            realmShapers.erase(realmShapers.begin() + i);
            return i;
        }
    }
    return -1;
}

int ShaperTree::findIndex(RealmShaper *shaper)
{
    // return index in the tree if found
    // else
    for (int i = 0; i < realmShapers.size(); i++) {
        if ( realmShapers[i] == shaper) {
            return i;
        }
    }

    return -1;
}

int ShaperTree::getDepth(RealmShaper *shaper)
{
    // return depth of the node in the tree if found
    // else
    for (int i = 0; i < realmShapers.size(); i++) {
        if (realmShapers[i] == shaper) {
            // Depth is based on the index in a level-order representation
            return static_cast<int>(std::log2(i + 1));
        }
    }
    
    return -1;
}

int ShaperTree::getDepth()
{
    // return total|max depth|height of the tree
    int depth = log2(realmShapers.size()-1); // Exclude extra 1 in size function.
    return depth;
}

RealmShaper ShaperTree::duel(RealmShaper *challenger, bool result)
{
    // TODO: Implement duel logic, return the victor
    // Use   std::cout << "[Duel] " << victorName << " won the duel" << std::endl;
    // Use   std::cout << "[Honour] " << "New honour points: ";
    // Use   std::cout << challengerName << "-" << challengerHonour << " ";
    // Use   std::cout << opponentName << "-" << opponentHonour << std::endl;
    // Use   std::cout << "[Duel] " << loserName << " lost all honour, delete" << std::endl;
    RealmShaper *parent = getParent(challenger);
    if (result) {
        challenger->gainHonour();
        parent->loseHonour();
        replace(challenger, parent);
        std::cout << "[Duel] " << challenger->getName() << " won the duel" << std::endl;
        std::cout << "[Honour] " << "New honour points: ";
        std::cout << challenger->getName() << "-" << challenger->getHonour() << " ";
        std::cout << parent->getName() << "-" << parent->getHonour() << std::endl;
        if (parent->getHonour() <= 0) {
            std::cout << "[Duel] " << parent->getName() << " lost all honour, delete" << std::endl;
            int parentIndex = findIndex(parent);
            delete realmShapers[parentIndex];
            realmShapers.erase(realmShapers.begin() + parentIndex);
        }
        return *challenger;
    } else {
        challenger->loseHonour();
        parent->gainHonour();
        std::cout << "[Duel] " << challenger->getName() << " lost the duel" << std::endl;
        std::cout << "[Honour] " << "New honour points: ";
        std::cout << challenger->getName() << "-" << challenger->getHonour() << " ";
        std::cout << parent->getName() << "-" << parent->getHonour() << std::endl;
        if (challenger->getHonour() <= 0) {
            std::cout << "[Duel] " << challenger->getName() << " lost all honour, delete" << std::endl;
            int challengerIndex = findIndex(challenger);
            delete realmShapers[challengerIndex];
            realmShapers.erase(realmShapers.begin() + challengerIndex);
        }
        return *parent;
    }
}

RealmShaper *ShaperTree::getParent(RealmShaper *shaper)
{
    RealmShaper *parent = nullptr;

    // TODO: return parent of the shaper
    int shaperIndex = findIndex(shaper);
    int parentIndex = (shaperIndex-1)/2;
    parent = realmShapers[parentIndex];

    return parent;
}

void ShaperTree::replace(RealmShaper *player_low, RealmShaper *player_high)
{
    // TODO: Change player_low and player_high's positions on the tree
    int player_lowIndex = findIndex(player_low);
    int player_highIndex = findIndex(player_high);
    std::swap(realmShapers[player_lowIndex], realmShapers[player_highIndex]);
}

RealmShaper *ShaperTree::findPlayer(RealmShaper shaper)
{
    RealmShaper *foundShaper = nullptr;

    // TODO: Search shaper by object
    // Return the shaper if found
    // Return nullptr if shaper not found

    
    for (RealmShaper* object : realmShapers) {
        if (*object == shaper) {
            foundShaper = object;
        }
    }

    // If no match is found, return nullptr
    return foundShaper;
}

// Find shaper by name
RealmShaper *ShaperTree::findPlayer(std::string name)
{
    RealmShaper *foundShaper = nullptr;

    // TODO: Search shaper by name
    // Return the shaper if found
    // Return nullptr if shaper not found

    for (RealmShaper* object : realmShapers) {
        if (object->getName() == name) {
            foundShaper = object;
        }
    }

    return foundShaper;
}

std::vector<std::string> ShaperTree::inOrderTraversal(int index)
{
    std::vector<std::string> result = {};
    // TODO: Implement inOrderTraversal in tree
    // Add all to a string vector
    // Return the vector

    // Define and implement as many helper functions as necessary for recursive implementation

    // Note: Since SheperTree is not an binary search tree,
    // in-order traversal will not give rankings in correct order
    // for correct order you need to implement level-order traversal
    // still you are to implement this function as well
    if (index >= realmShapers.size()) {
        return result; // Return if the index is invalid
    }


    RealmShaper* temp = realmShapers[index];
    std::vector<std::string> recursiveResult;
    if(temp != nullptr) {
        recursiveResult = inOrderTraversal(2*index+1);
        result.insert(result.end(), recursiveResult.begin(), recursiveResult.end());
        result.push_back(temp->getName());
        recursiveResult = inOrderTraversal(2*index+2);
        result.insert(result.end(), recursiveResult.begin(), recursiveResult.end());
    }

    return result;
}

std::vector<std::string> ShaperTree::preOrderTraversal(int index)
{
    std::vector<std::string> result = {};
    // TODO: Implement preOrderTraversal in tree
    // Add all to a string vector
    // Return the vector

    // Define and implement as many helper functions as necessary for recursive implementation
    
    if (index >= realmShapers.size()) {
        return result; // Return if the index is invalid
    }
    
    RealmShaper* temp = realmShapers[index];
    std::vector<std::string> recursiveResult;
    if (temp != nullptr) {
        result.push_back(temp->getName());
        recursiveResult = preOrderTraversal(2*index+1);
        result.insert(result.end(), recursiveResult.begin(), recursiveResult.end()); 
        recursiveResult = preOrderTraversal(2*index+2);
        result.insert(result.end(), recursiveResult.begin(), recursiveResult.end());
    }

    return result;
}

std::vector<std::string> ShaperTree::postOrderTraversal(int index)
{
    std::vector<std::string> result = {};
    // TODO: Implement postOrderTraversal in tree
    // Add all to a string vector
    // Return the vector

    // Define and implement as many helper functions as necessary for recursive implementation
    
    if (index >= realmShapers.size()) {
        return result; // Return if the index is invalid
    }

    RealmShaper* temp = realmShapers[index];
    std::vector<std::string> recursiveResult;
    if (temp != nullptr) {
        recursiveResult = postOrderTraversal(2*index+1);
        result.insert(result.end(), recursiveResult.begin(), recursiveResult.end()); 
        recursiveResult = postOrderTraversal(2*index+2);
        result.insert(result.end(), recursiveResult.begin(), recursiveResult.end());
        result.push_back(temp->getName());
    }

    return result;
}

void ShaperTree::preOrderTraversal(int index, std::ofstream &outFile)
{
    // TODO: Implement preOrderTraversal in tree
    // write nodes to output file

    // Define and implement as many helper functions as necessary for recursive implementation
    std::vector<std::string> preOrderVector = preOrderTraversal(index);
    for (std::string shaper : preOrderVector) {
        outFile << shaper << "\n";
    }
}

void ShaperTree::breadthFirstTraversal(std::ofstream &outFile)
{
    // TODO: Implement level-order traversal
    // write nodes to output file

    // Define and implement as many helper functions as necessary
    for (RealmShaper* shaper : realmShapers) {
        outFile << shaper->getName() << "\n";
    }
}

void ShaperTree::displayTree()
{
    std::cout << "[Shaper Tree]" << std::endl;
    printTree(0, 0, "");
}

// Helper function to print tree with indentation
void ShaperTree::printTree(int index, int level, const std::string &prefix)
{
    if (!isValidIndex(index))
        return;

    std::cout << prefix << (level > 0 ? "   └---- " : "") << *realmShapers[index] << std::endl;
    int left = 2*index+1;  // TODO: Calculate left index
    int right = 2*index+2; // TODO: Calculate right index

    if (isValidIndex(left) || isValidIndex(right))
    {
        printTree(left, level + 1, prefix + (level > 0 ? "   │   " : "")); // ╎
        printTree(right, level + 1, prefix + (level > 0 ? "   │   " : ""));
    }
}

void ShaperTree::writeShapersToFile(const std::string &filename)
{
    // TODO: Write the shapers to filename output level by level
    // Use std::cout << "[Output] " << "Shapers have been written to " << filename << " according to rankings." << std::endl;
    std::ofstream outputFile(filename);
    breadthFirstTraversal(outputFile);
    std::cout << "[Output] " << "Shapers have been written to " << filename << " according to rankings." << std::endl;
}

void ShaperTree::writeToFile(const std::string &filename)
{
    // TODO: Write the tree to filename output pre-order
    // Use std::cout << "[Output] " << "Tree have been written to " << filename << " in pre-order." << std::endl;
    std::ofstream outputFile(filename);
    preOrderTraversal(0, outputFile);
    std::cout << "[Output] " << "Tree have been written to " << filename << " in pre-order." << std::endl;
}
