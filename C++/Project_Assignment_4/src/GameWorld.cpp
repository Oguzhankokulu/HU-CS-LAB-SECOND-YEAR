#include "GameWorld.h"
#include "Map.h"

GameWorld::GameWorld() : mapTree(), shaperTree() {}

void GameWorld::initializeGame(std::vector<Isle *> places, std::vector<RealmShaper *> players)
{
    shaperTree.initializeTree(players);
    mapTree.setIsInitialize(true);
    mapTree.initializeMap(places);
    mapTree.setIsInitialize(false);
    mapTree.setRotationCount(0);
}

Map& GameWorld::getMapTree()
{
    return mapTree;
}
ShaperTree& GameWorld::getShaperTree()
{
    return shaperTree;
}

bool GameWorld::hasAccess(RealmShaper *realmShaper, Isle *isle)
{
    bool hasAccess = false;

    // TODO: Check if the realmShaper has access to explore the isle
    // Get necessary depth values
    // Use mapTree.calculateMinMapDepthAccess
    // Use // std::cout << "[Access Control] " << "RealmShaper not found!" << std::endl;

    int playerDepth = shaperTree.getDepth(realmShaper);
    if (playerDepth == -1) {
        std::cout << "[Access Control] " << "RealmShaper not found!" << std::endl;
        return false;
    }
    int totalShaperTreeHeight = shaperTree.getDepth();
    int totalMapDepth = mapTree.getDepth();
    int isleDepth;
    if (mapTree.findIsle(isle->getName()) == nullptr) {
        isleDepth = mapTree.getInsertionDepth(isle->getName());
    } else {
        isleDepth = mapTree.getIsleDepth(isle);
    }
    
    int minMapDepthAccess = mapTree.calculateMinMapDepthAccess(playerDepth, totalShaperTreeHeight, totalMapDepth);

    if (isleDepth >= minMapDepthAccess)
        hasAccess = true;

    return hasAccess;
}

void GameWorld::exploreArea(RealmShaper *realmShaper, Isle *isle)
{
    // TODO:
    // Check if realmShaper has access
    // Use // std::cout << "[Explore Area] " << realmShaper->getName() << " does not have access to explore area " << *isle << std::endl;
    // If realmShaper has access
    // Visit isle, 
    // collect item, 
    // check overcrowding for Isle, 
    // delete Isle if necessary

    // Use // std::cout << "[Explore Area] " << realmShaper->getName() << " visited " << isle->getName() << std::endl;
    // Use // std::cout << "[Energy] " << realmShaper->getName() << "'s new energy level is " << realmShaper->getEnergyLevel() << std::endl;
    // Use // std::cout << "[Owercrowding] " << isle->getName() << " self-destructed, it will be removed from the map" << std::endl;

    // You will need to implement a mechanism to keep track of how many realm shapers are at an Isle at the same time
    // There are more than one ways to do this, so it has been left completely to you
    // Use shaperCount, but that alone will not be enough,
    // you will likely need to add attributes that are not currently defined
    // to RealmShaper or Isle or other classes depending on your implementation
    bool access = hasAccess(realmShaper, isle);

    if (!access) {
        std::cout << "[Explore Area] " << realmShaper->getName() << " does not have access to explore area " << *isle << "." << std::endl;
        return;
    }

    realmShaper->collectItem(isle->getItem());
    std::cout << "[Explore Area] " << realmShaper->getName() << " visited " << isle->getName() << std::endl;
    std::cout << "[Energy] " << realmShaper->getName() << "'s new energy level is " << realmShaper->getEnergyLevel() << std::endl;

    Isle* prevIsle = realmShaper->getCurrentIsle(); 
    if (prevIsle != nullptr && mapTree.doesContain(prevIsle, mapTree.getRoot())) {
        prevIsle->decreaseShaperCount();
    }

    realmShaper->setCurrentIsle(isle);
    bool isExceed = isle->increaseShaperCount();
    if (isExceed) {
        std::cout << "[Owercrowding] " << isle->getName() << " self-destructed, it will be removed from the map" << std::endl;
        mapTree.remove(isle);
        isle = nullptr;  
    }
    
}

void GameWorld::craft(RealmShaper *shaper, const std::string &isleName){
    // TODO: Check energy and craft new isle if possible
    // Use std::cout << "[Energy] " << shaperName << " has enough energy points: " << shaperEnergyLevel << std::endl;
    // Use std::cout << "[Craft] " << shaperName << " crafted new Isle " << isleName << std::endl;
    // Use std::cout << "[Energy] " << shaperName << " does not have enough energy points: " << shaperEnergyLevel << std::endl;

    // Check if the shaper has access to the new crafted isle


    bool hasEnergy = shaper->hasEnoughEnergy();

    if (hasEnergy) {
        std::cout << "[Energy] " << shaper->getName() << " has enough energy points: " << shaper->getEnergyLevel() << std::endl;
        Isle* isle = new Isle(isleName);
        mapTree.insert(isle);
        shaper->loseEnergy();
        std::cout << "[Craft] " << shaper->getName() << " crafted new Isle " << isleName << std::endl;
    } else {
        std::cout << "[Energy] " << shaper->getName() << " does not have enough energy points: " << shaper->getEnergyLevel() << std::endl;
    }
}

void GameWorld::displayGameState()
{
    //mapTree.displayMap();
    //shaperTree.displayTree();
}

// TODO: Implement functions to read and parse Access and Duel logs

void GameWorld::processGameEvents(const std::string &accessLogs, const std::string &duelLogs)
{
    // TODO:
    // Read logs
    // For every 5 access, 1 duel happens
    // If there are still duel logs left after every access happens duels happens one after other

    // This function should call exploreArea and craft functions

    // Use displayGameState();

    std::ifstream access(accessLogs);
    std::ifstream duel(duelLogs);

    std::string accessLine;
    std::string duelLine;
    int count = 0;
    getline(access, accessLine);
    getline(duel, duelLine);
    while(getline(access, accessLine)) {
        std::istringstream strStream(accessLine);
        std::string playerName;
        getline(strStream, playerName, ' ');
        std::string isleName;
        strStream >> isleName;

        Isle* isle = mapTree.findIsle(isleName);
        RealmShaper* shaper = shaperTree.findPlayer(playerName);

        if (isle == nullptr) {
            craft(shaper, isleName);
        } else {
            exploreArea(shaper, isle);
        }

        count++;
        if (count == 5) {
            getline(duel, duelLine);
            std::istringstream duelStream(duelLine);
            std::string duelPlayerName;
            bool result;
            duelStream >> duelPlayerName;
            duelStream >> result;
            RealmShaper* duelShaper = shaperTree.findPlayer(duelPlayerName);
            if (duelShaper != nullptr)
                shaperTree.duel(duelShaper, result);
            count = 0;
        }
    }

    while (getline(duel, duelLine)) {
        std::istringstream duelStream(duelLine);
        std::string duelPlayerName;
        bool result;
        duelStream >> duelPlayerName;
        duelStream >> result;
        RealmShaper* duelShaper = shaperTree.findPlayer(duelPlayerName);
        if (duelShaper != nullptr)
            shaperTree.duel(duelShaper, result);
    }

    // displayGameState();
    // access.close();
    // duel.close();
}

void GameWorld::saveGameState(const std::string &currentIsles, const std::string &currentWorld, const std::string &currentShapers, const std::string &currentPlayerTree)
{
    mapTree.writeIslesToFile(currentIsles);
    mapTree.writeToFile(currentWorld);
    shaperTree.writeToFile(currentPlayerTree);
    shaperTree.writeShapersToFile(currentShapers);
}