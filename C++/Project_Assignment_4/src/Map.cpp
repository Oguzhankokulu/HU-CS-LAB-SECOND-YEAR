#include "Map.h"

Map::Map()
{
    this->root = nullptr;
}
Map::~Map()
{
    // TODO: Free any dynamically allocated memory if necessary
    deleteMap(root);
}

void Map::deleteMap(MapNode* mapNode) {
    if (mapNode == nullptr) {
        return;
    }
    
    deleteMap(mapNode->left);
    deleteMap(mapNode->right);
    delete mapNode;
}

void Map::initializeMap(std::vector<Isle *> isles)
{
    // TODO: Insert innitial isles to the tree
    // Then populate with Goldium and Einstainium items
    for (Isle* isle : isles) {
        insert(isle);
    }
    populateWithItems();
}

MapNode *Map::rotateRight(MapNode *current)
{
    // TODO: Perform right rotation according to AVL
    // return necessary new root
    // Use std::cerr << "[Right Rotation] " << "Called on invalid node!" << std::endl;
    if (current == nullptr) {
        std::cerr << "[Right Rotation] " << "Called on invalid node!" << std::endl;
    }
    
    
    MapNode* currentLeft = current->left;
    MapNode* transferredNode = currentLeft->right;

    currentLeft->right = current;
    current->left = transferredNode;

    current->height = std::max(height(current->left), height(current->right)) + 1;
    currentLeft->height = std::max(height(currentLeft->left), height(currentLeft->right)) + 1;


    return currentLeft;
}

MapNode *Map::rotateLeft(MapNode *current)
{
    // TODO: Perform left rotation according to AVL
    // return necessary new root
    // Use std::cerr << "[Left Rotation] " << "Called on invalid node!" << std::endl;
    if (current == nullptr) {
        std::cerr << "[Left Rotation] " << "Called on invalid node!" << std::endl;
    }

    MapNode* currentRight = current->right;
    MapNode* transferredNode = currentRight->left;

    currentRight->left = current;
    current->right = transferredNode;

    current->height = std::max(height(current->left), height(current->right)) + 1;
    currentRight->height = std::max(height(currentRight->left), height(currentRight->right)) + 1;


    return currentRight;
}

int Map::calculateMinMapDepthAccess(int playerDepth, int totalShaperTreeHeight, int totalMapDepth)
{
    return (int)totalMapDepth * ((double)playerDepth / totalShaperTreeHeight);
}

int Map::height(MapNode *node)
{
    // TODO: Return height of the node
    return node == nullptr ? -1 : node->height;
}

MapNode *Map::insert(MapNode *node, Isle *isle)
{
    // TODO: Recursively insert isle to the tree
    // returns inserted node

    // If the node is null, create a new node
    if (node == nullptr) {
        return new MapNode(isle);
    }

    std::string name = node->isle->getName();

    // Recursively insert
    if (name < isle->getName()) {
        node->right = insert(node->right, isle);
    } else if (name > isle->getName()) {
        node->left = insert(node->left, isle);
    }

    node->height = 1 + std::max(height(node->left), height(node->right));

    int difference = height(node->left) - height(node->right);
    if (difference > 1 && isle->getName() < node->left->isle->getName()) {
        rotationCount++;
        return rotateRight(node);
    } else if (difference > 1 && isle->getName() > node->left->isle->getName()) {
        rotationCount++;
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    } else if (difference < -1 && isle->getName() > node->right->isle->getName()) {
        rotationCount++;
        return rotateLeft(node);
    } else if (difference < -1 && isle->getName() < node->right->isle->getName()) {
        rotationCount++;
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

void Map::insert(Isle *isle)
{
    root = insert(root, isle);
    
    if (getRotationCount() > 0 && !isInitialize) {
        totalRotationCount += 1;
        setRotationCount(0);
        if (totalRotationCount == 3) {
            //mapTree.resetMapItems(mapTree.getRoot());
            populateWithItems();
            dropItemBFS();
            totalRotationCount = 0;
        }
    }
    // you might need to insert some checks / functions here depending on your implementation
}

MapNode* Map::remove(MapNode* node, Isle* isle) {
    if (node == nullptr) {
        return nullptr; // Nothing to delete
    }

    // Traverse
    std::string name = node->isle->getName();
    if (name == isle->getName()) {
        // Node with matching isle found

        // Leaf node deletion (no children)
        if (node->left == nullptr && node->right == nullptr) {
            delete node;
            return nullptr;
        }
        
        // Node with one child (left or right)
        else if (node->left == nullptr) {
            MapNode* temp = node->right;
            delete node;
            return temp;
        } 
        else if (node->right == nullptr) {
            MapNode* temp = node->left;
            delete node;
            return temp;
        }

        // Node with two children, replace with inorder predecessor
        else {
            // Find the inorder predecessor (rightmost node in the left subtree)
            MapNode* inOrderPredecessor = node->left;
            while (inOrderPredecessor->right != nullptr) {
                inOrderPredecessor = inOrderPredecessor->right;
            }

            // Replace
            Isle* newIsle = new Isle(inOrderPredecessor->isle->getName());
            node->isle = newIsle;

            // Recursively remove the inorder predecessor from the left subtree
            node->left = remove(node->left, inOrderPredecessor->isle);
        }
    } else if (name < isle->getName()) {
        node->right = remove(node->right, isle);
    } else {
        node->left = remove(node->left, isle);
    }

    // Update the height of the current node
    if (node != nullptr) {
        node->height = std::max(height(node->left), height(node->right)) + 1;
    }

    // Rebalance the tree if necessary (check balance factor)
    int balance = height(node->left) - height(node->right);

    // Left high
    if (balance > 1) {
        // Left-left case
        if (height(node->left->left) >= height(node->left->right)) {
            return rotateRight(node);
        }
        // Left-right case
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // Right high
    if (balance < -1) {
        // Right-right case
        if (height(node->right->right) >= height(node->right->left)) {
            return rotateLeft(node);
        }
        // Right-left case
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;  // Return rebalanced node
}

void Map::remove(Isle* isle) {
    if (root == nullptr) {
        return;
    }
    root = remove(root, isle);  // Start the removal process from the root

    if (getRotationCount() > 0 && !isInitialize) {
        totalRotationCount++;
        setRotationCount(0);
        if (totalRotationCount == 3) {
            populateWithItems();
            dropItemBFS();
            totalRotationCount = 0;
        }
    }
}


void Map::setIsInitialize(bool val) {
    this->isInitialize = val;
}

void Map::preOrderItemDrop(MapNode *current, int &count)
{
    // TODO: Drop EINSTEINIUM according to rules
    // Use std::cout << "[Item Drop] " << "EINSTEINIUM dropped on Isle: " << current->isle->getName() << std::endl;
    if (current == nullptr) {
        return;
    }
    if (++count == 5) {
        current->isle->setItem(EINSTEINIUM);
        count = 0;
        std::cout << "[Item Drop] " << "EINSTEINIUM dropped on Isle: " << current->isle->getName() << std::endl;
    }
    preOrderItemDrop(current->left, count);
    preOrderItemDrop(current->right, count);
}

// to Display the values by Post Order Method .. left - right - node
void Map::postOrderItemDrop(MapNode *current, int &count)
{
    // TODO: Drop GOLDIUM according to rules
    // Use  std::cout << "[Item Drop] " << "GOLDIUM dropped on Isle: " << current->isle->getName() << std::endl;
    if (current == nullptr) {
        return;
    }
    postOrderItemDrop(current->left, count);
    postOrderItemDrop(current->right, count);
    if (++count == 3) {
        current->isle->setItem(GOLDIUM);
        count = 0;
        std::cout << "[Item Drop] " << "GOLDIUM dropped on Isle: " << current->isle->getName() << std::endl;
    }
}

MapNode *Map::findFirstEmptyIsle(MapNode *node)
{
    // TODO: Find first Isle with no item

    if (node == nullptr) { // If the tree is empty
        return nullptr;
    }

    // A queue to implement level order traversal
    std::queue<MapNode*> queue;
    queue.push(node);

    while (!queue.empty()) {
        MapNode* current = queue.front();
        queue.pop();

        // Check if the current Isle is empty
        if (current->isle->getItem() == EMPTY) {
            return current;
        }

        // Add left and right children to the queue if they exist
        if (current->left != nullptr) {
            queue.push(current->left);
        }
        if (current->right != nullptr) {
            queue.push(current->right);
        }
    }

    // If no empty isle is found, return nullptr
    return nullptr;
}

void Map::dropItemBFS()
{
    // TODO: Drop AMAZONITE according to rules
    // Use std::cout << "[BFS Drop] " << "AMAZONITE dropped on Isle: " << targetNode->isle->getName() << std::endl;
    // Use std::cout << "[BFS Drop] " << "No eligible Isle found for AMAZONITE drop." << std::endl;

    MapNode* targetNode = findFirstEmptyIsle(root);

    if (targetNode == nullptr) {
        std::cout << "[BFS Drop] " << "No eligible Isle found for AMAZONITE drop." << std::endl;
        return;
    } else {
        std::cout << "[BFS Drop] " << "AMAZONITE dropped on Isle: " << targetNode->isle->getName() << std::endl;
        targetNode->isle->setItem(AMAZONITE);
    }
}

void Map::displayMap()
{
    std::cout << "[World Map]" << std::endl;
    display(root, 0, 0);
}

bool Map::doesContain(Isle* isle, MapNode* node) {
    if (node == nullptr) {
        return false;
    } else if (node->isle == isle) {
        return true;
    } else {
        return doesContain(isle, node->left) || doesContain(isle, node->right);
    }
}

MapNode* Map::getRoot() {
    return this->root;
}

void Map::resetMapItems(MapNode* node) { // preorder traversal.
    if (node == nullptr) return;
    node->isle->setItem(EMPTY);
    resetMapItems(node->left); // Recur on the left subtree
    resetMapItems(node->right); // Recur on the right subtree 
}

int Map::getRotationCount() {
    return this->rotationCount;
}

void Map::setRotationCount(int count) {
    this->rotationCount = count;
}

int Map::getDepth(MapNode *node)
{
    // TODO: Return node depth if found, else
    int depth = 0;
    MapNode* temp = root;
    while (temp != nullptr) {
        if (temp->isle->getName() == node->isle->getName()) {
            return depth;
        } else if (temp->isle->getName() < node->isle->getName()) {
            temp = temp->right;
            ++depth;
        } else {
            temp = temp->left;
            ++depth;
        }
    }
    return -1;
}

// Function to calculate the depth of a specific node in the AVL tree
int Map::getIsleDepth(Isle *isle)
{
    // TODO: Return node depth by isle if found, else
    int depth = 0;
    MapNode* temp = root;
    while (temp != nullptr) {
        if (temp->isle->getName() == isle->getName()) {
            return depth;
        } else if (temp->isle->getName() < isle->getName()) {
            temp = temp->right;
            ++depth;
        } else {
            temp = temp->left;
            ++depth;
        }
    }
    return -1;
}

int Map::getDepth()
{
    // TODO: Return max|total depth of tree
    if (root == nullptr) {
        return -1;
    }

    return root->height;
}

int Map::getInsertionDepth(std::string isleName) {

    MapNode* temp = root;
    while (true) {
        if (temp->isle->getName() < isleName) {
            if (temp->right == nullptr) {
                return temp->height + 1;
            }
            temp = temp->right;
        } else {
            if (temp->left == nullptr) {
                return temp->height + 1;
            }
            temp = temp->left;
        }
    }
}

void Map::populateWithItems()
{
    // TODO: Distribute fist GOLDIUM than EINSTEINIUM
    int startingCount = 0;
    postOrderItemDrop(root,startingCount);
    startingCount = 0;
    preOrderItemDrop(root,startingCount);
}

Isle *Map::findIsle(Isle isle)
{
    MapNode* temp = root;
    while (temp != nullptr) {
        if (temp->isle->getName() == isle.getName()) {
            return temp->isle;
        } else if (temp->isle->getName() < isle.getName()) {
            temp = temp->right;
        } else {
            temp = temp->left;
        } 
    }
    return nullptr;
}

Isle *Map::findIsle(std::string name)
{
    // TODO: Find isle by name
    MapNode* temp = root;
    while (temp != nullptr) {
        if (temp->isle->getName() == name) {
            return temp->isle;
        } else if (temp->isle->getName() < name) {
            temp = temp->right;
        } else {
            temp = temp->left;
        } 
    }
    return nullptr;
}

MapNode *Map::findNode(Isle isle)
{
    // TODO: Find node by value
    MapNode* temp = root;
    while (temp != nullptr) {
        if (temp->isle->getName() == isle.getName()) {
            return temp;
        } else if (temp->isle->getName() < isle.getName()) {
            temp = temp->right;
        } else {
            temp = temp->left;
        }
    }
    return nullptr;
}

MapNode *Map::findNode(std::string name)
{
    // TODO: Find node by name
    MapNode* temp = root;
    while (temp != nullptr) {
        if (temp->isle->getName() == name) {
            return temp;
        } else if (temp->isle->getName() < name) {
            temp = temp->right;
        } else {
            temp = temp->left;
        } 
    }
    return nullptr;
}

void Map::display(MapNode *current, int depth, int state)
{
    // SOURCE:

    if (current->left)
        display(current->left, depth + 1, 1);

    for (int i = 0; i < depth; i++)
        printf("     ");

    if (state == 1) // left
        printf("   ┌───");
    else if (state == 2) // right
        printf("   └───");

    std::cout << "[" << *current->isle << "] - (" << current->height << ")\n"
              << std::endl;

    if (current->right)
        display(current->right, depth + 1, 2);
}

void Map::writeToFile(const std::string &filename)
{
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << "\n";
        return;
    }

    // Check if the tree is empty
    if (root == nullptr) {
        file << "NULL" << std::endl;
        file.close();
        return;
    }

    // Initialize a queue for level-order traversal
    std::queue<MapNode*> queue;
    queue.push(root);

    int height = root->height;
    int numberOfElements = pow(2, height + 1);

    // Initialize the array with nullptr
    MapNode* arr[numberOfElements] = {nullptr};
    arr[0] = root;  // Set the root node in the array
    int index = 0;

    for (int i = 0; i < numberOfElements; i++) {
        // Ensure the queue is not empty before accessing
        if (!queue.empty()) {
            MapNode* temp = queue.front();
            queue.pop();

            if (temp != nullptr) {
                int leftIndex = 2 * index + 1;
                int rightIndex = 2 * index + 2;

                // Push left child if within bounds
                if (leftIndex < numberOfElements) {
                    arr[leftIndex] = temp->left;
                    queue.push(temp->left);
                }

                // Push right child if within bounds
                if (rightIndex < numberOfElements) {
                    arr[rightIndex] = temp->right;
                    queue.push(temp->right);
                }
            }
        }
        index++;
    }

    // Write tree structure to the file
    for (int i = 1; i <= height + 1; i++) {
        for (int j = pow(2, i - 1) - 1; j < pow(2, i) - 1; j++) {
            if (arr[j] == nullptr) {
                file << "NULL ";
            } else {
                file << arr[j]->isle->getName() << " ";
            }
        }
        file << std::endl;
    }

    file.close();
}


void Map::writeIslesToFile(const std::string &filename)
{
    // TODO: Write Isles to output file in alphabetical order
    // Use std::cout << "[Output] " << "Isles have been written to " << filename << " in in alphabetical order." << std::endl;
    
    std::ofstream file(filename);

    std::vector<MapNode*> vector;
    inorderTraversal(root, vector);
    
    for (const auto& node : vector) {
        file << node->isle->getName() << std::endl;
    }

    file.close();
    std::cout << "[Output] " << "Isles have been written to " << filename << " in in alphabetical order." << std::endl;
}

void Map::inorderTraversal(MapNode* node, std::vector<MapNode*>& treeVector) {
    if (node == nullptr) {
        return;
    }

    // Traverse left subtree
    inorderTraversal(node->left, treeVector);

    // Visit the current node (add it to the vector)
    treeVector.push_back(node);

    // Traverse right subtree
    inorderTraversal(node->right, treeVector);
}