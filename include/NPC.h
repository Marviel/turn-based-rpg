#ifndef NPC_H
#define NPC_H

#include <string>
#include <vector>
#include <map>

class NPC
{
private:
    std::string name;
    std::string emoji;
    std::vector<std::string> dialogues;
    std::map<std::string, int> shopItems; // item name -> price in BDP
    bool isShopkeeper;

public:
    NPC(const std::string &name, const std::string &emoji, bool isShopkeeper = false);

    // Getters
    std::string getName() const;
    std::string getEmoji() const;
    bool getIsShopkeeper() const;

    // Dialogue methods
    void addDialogue(const std::string &dialogue);
    std::string getRandomDialogue() const;

    // Shop methods
    void addShopItem(const std::string &itemName, int price);
    const std::map<std::string, int> &getShopItems() const;

    // Display methods
    void displayInfo() const;
};

#endif // NPC_H