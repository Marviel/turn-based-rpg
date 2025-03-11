#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include <map>
#include "Entity.h"

struct Item
{
    std::string name;
    std::string description;
    std::string emoji;
    bool isConsumable;

    Item(const std::string &name, const std::string &description, const std::string &emoji, bool isConsumable)
        : name(name), description(description), emoji(emoji), isConsumable(isConsumable) {}
};

class Player : public Entity
{
private:
    int level;
    int experience;
    int experienceToNextLevel;
    int bdp; // Big Daddy Points (currency)
    std::vector<Item> inventory;
    std::map<std::string, int> itemCounts; // Track quantity of each item

public:
    Player(const std::string &name);

    // Getters
    int getLevel() const;
    int getExperience() const;
    int getExperienceToNextLevel() const;
    int getBDP() const;
    const std::vector<Item> &getInventory() const;
    int getItemCount(const std::string &itemName) const;

    // Setters and modifiers
    void gainExperience(int amount);
    void levelUp();
    void earnBDP(int amount);
    void spendBDP(int amount);
    void addItem(const Item &item);
    bool useItem(const std::string &itemName);

    // Override methods from Entity
    void displayStats() const override;
    void displayInventory() const;
};

#endif // PLAYER_H