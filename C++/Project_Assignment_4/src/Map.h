#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <memory>
#include <vector>
#include <fstream>
#include <queue>
#include <cmath>
#include "Isle.h"

struct MapNode
{
    Isle *isle;
    MapNode *left, *right;
    int height = 0;

    MapNode(Isle *isle) : isle(isle), left(nullptr), right(nullptr), height(0) {}
    ~MapNode()
    {
        if (isle != nullptr) {
            delete isle;
        }
    }
};

class Map
{
private:
    MapNode *root; // Root node of the tree
    int rotationCount = 0;
    int totalRotationCount = 0;
    bool isInitialize = false;

    // Height of a node
    int height(MapNode *node);

    // AVL Rotations
    MapNode *rotateRight(MapNode *current);
    MapNode *rotateLeft(MapNode *current);

    // Helper functions for recursive AVL insertion and deletion
    // USE THEM
    MapNode *insert(MapNode *node, Isle *isle);
    MapNode *remove(MapNode *node, Isle *isle);

    // Item drop helper functions
    void preOrderItemDrop(MapNode *current, int &count);
    void postOrderItemDrop(MapNode *current, int &count);

    // Helper function to free dynamically allocated memory.
    void deleteMap(MapNode* root);

public:
    // Constructor declaration
    Map();
    // Destructor
    ~Map();

    // Calculation of minumum depth a player can access at the map
    int calculateMinMapDepthAccess(int playerDepth, int totalShaperTreeHeight, int totalMapDepth);

    // Tree operations
    void insert(Isle *isle);
    void remove(Isle *isle);
    /// Search
    Isle *findIsle(Isle isle);
    Isle *findIsle(std::string name);
    MapNode *findNode(Isle isle);
    MapNode *findNode(std::string name);

    // Initilize tree from a vector
    void initializeMap(std::vector<Isle *> isles);

    MapNode *findFirstEmptyIsle(MapNode *node);
    
    void setIsInitialize(bool val);
    bool doesContain(Isle* isle, MapNode* node);
    MapNode* getRoot();
    void resetMapItems(MapNode* node);
    int getRotationCount();
    void setRotationCount(int count);
    int getDepth(MapNode *node); // Determines the depth of a node within the tree.
    int getIsleDepth(Isle *isle); // Determines the depth of a isle within the tree.
    int getDepth(); // Total level of the tree
    int getInsertionDepth(std::string isleName);

    // Display tree in terminal
    void display(MapNode *current, int depth, int state);
    void displayMap();

    // Item drop api
    void populateWithItems();
    void dropItemBFS();


    // Write the AVL tree to a file with levels
    void writeToFile(const std::string &filename);
    // Write current Isles to file in alphabetical order
    void writeIslesToFile(const std::string &filename);
    // In Order Traversal function
    void inorderTraversal(MapNode* node, std::vector<MapNode*>& isles);
};

#endif