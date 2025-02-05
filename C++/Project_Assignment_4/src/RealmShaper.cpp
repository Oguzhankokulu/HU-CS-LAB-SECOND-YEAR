#include "RealmShaper.h"
#include <fstream>
#include <sstream>
#include <iostream>

#define NECESSARY_ENERGY 2000 // Necessary energy to craft an Isle

RealmShaper::RealmShaper(std::string name, int honour) : name(name)
{
    this->honour = honour;
}

const std::string &RealmShaper::getName() const
{
    return this->name;
}

int RealmShaper::getEnergyLevel()
{
    return this->collectedEnergyPoints;
}

int RealmShaper::getHonour()
{
    return this->honour;
}

Isle* RealmShaper::getCurrentIsle() {
    return this->currentIsle;
}

void RealmShaper::setCurrentIsle(Isle* isle) {
    this->currentIsle = isle;
}

void RealmShaper::gainHonour()
{
    this->honour += 150;
}

void RealmShaper::loseHonour()
{
    this->honour -= 150;
}

void RealmShaper::collectItem(Item item)
{
    this->collectedEnergyPoints += item;
}

void RealmShaper::loseEnergy()
{
    this->collectedEnergyPoints -= NECESSARY_ENERGY;
}

bool RealmShaper::hasEnoughEnergy()
{
    return this->collectedEnergyPoints >= NECESSARY_ENERGY;
}

std::vector<RealmShaper *> RealmShaper::readFromFile(const std::string &filename)
{
    std::vector<RealmShaper *> players;

    // TODO: Read RealmShapers from the file,
    // add them to vector
    // return the vector
    // Input format: playerName[tab]honourPoints

    std::ifstream file(filename);

    std::string line;
    while (getline(file, line)) {
        std::istringstream strStream(line);
        std::string playerName;
        int honourPoints = 1000;
        getline(strStream, playerName, ' ');
        strStream >> honourPoints;
        RealmShaper* realmShaper = new RealmShaper(playerName, honourPoints);
        players.push_back(realmShaper);
    }

    return players;
}

bool RealmShaper::operator==(const RealmShaper &other) const
{
    // TODO: Compare by name, return true if same
    return name == other.name;
}

std::ostream &operator<<(std::ostream &os, const RealmShaper &p)
{
    // Outs the player name
    return (os << p.name); // << std::endl);
}
