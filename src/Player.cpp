#include "Player.h"
#include <iostream>
#include <algorithm>

Player::Player(const std::string &name)
    : Entity(name, 100, 10, 5), level(1), experience(0), experienceToNextLevel(100), bdp(0) {}

int Player::getLevel() const
{
    return level;
}

int Player::getExperience() const
{
    return experience;
}

int Player::getExperienceToNextLevel() const
{
    return experienceToNextLevel;
}

int Player::getBDP() const
{
    return bdp;
}

const std::vector<Item> &Player::getInventory() const
{
    return inventory;
}

int Player::getItemCount(const std::string &itemName) const
{
    auto it = itemCounts.find(itemName);
    if (it != itemCounts.end())
    {
        return it->second;
    }
    return 0;
}

void Player::gainExperience(int amount)
{
    experience += amount;
    std::cout << "You gained " << amount << " experience! 📈" << std::endl;

    // Check if player can level up
    if (experience >= experienceToNextLevel)
    {
        levelUp();
    }
}

void Player::levelUp()
{
    level++;
    experience -= experienceToNextLevel;
    experienceToNextLevel = 100 * level; // Increase XP needed for next level

    // Increase stats
    maxHealth += 20;
    health = maxHealth; // Fully heal on level up
    attack += 5;
    defense += 2;

    std::cout << "\n🎉 LEVEL UP! 🎉" << std::endl;
    std::cout << "You are now level " << level << "!" << std::endl;
    std::cout << "Your stats have increased!" << std::endl;
    displayStats();
    std::cout << std::endl;
}

void Player::earnBDP(int amount)
{
    bdp += amount;
    std::cout << "You earned " << amount << " BDP! 💰" << std::endl;
}

void Player::spendBDP(int amount)
{
    if (bdp >= amount)
    {
        bdp -= amount;
        std::cout << "You spent " << amount << " BDP. Remaining: " << bdp << " BDP 💸" << std::endl;
    }
    else
    {
        std::cout << "Not enough BDP! You need " << amount << " but only have " << bdp << " 😢" << std::endl;
    }
}

void Player::addItem(const Item &item)
{
    // Add item to inventory
    inventory.push_back(item);

    // Update item count
    itemCounts[item.name]++;

    std::cout << "Added " << item.emoji << " " << item.name << " to your inventory!" << std::endl;
}

bool Player::useItem(const std::string &itemName)
{
    // Check if player has the item
    if (getItemCount(itemName) <= 0)
    {
        std::cout << "You don't have any " << itemName << "!" << std::endl;
        return false;
    }

    // Find the item in inventory
    auto it = std::find_if(inventory.begin(), inventory.end(),
                           [&itemName](const Item &item)
                           { return item.name == itemName; });

    if (it != inventory.end())
    {
        // Apply item effects
        if (itemName == "Health Potion")
        {
            int healAmount = getMaxHealth() / 2; // Heal 50% of max health
            heal(healAmount);
        }
        else if (itemName == "Attack Boost")
        {
            attack += 5;
            std::cout << "Your attack power has increased by 5! 💪" << std::endl;
        }
        else if (itemName == "Defense Boost")
        {
            defense += 3;
            std::cout << "Your defense has increased by 3! 🛡️" << std::endl;
        }

        // Remove item if consumable
        if (it->isConsumable)
        {
            // Decrease item count
            itemCounts[itemName]--;

            // Remove from inventory if count is 0
            if (itemCounts[itemName] <= 0)
            {
                inventory.erase(it);
                itemCounts.erase(itemName);
            }
        }

        return true;
    }

    return false;
}

void Player::displayStats() const
{
    std::cout << "👤 " << name << " (Level " << level << ")" << std::endl;
    std::cout << "❤️ Health: " << health << "/" << maxHealth << std::endl;
    std::cout << "⚔️ Attack: " << attack << std::endl;
    std::cout << "🛡️ Defense: " << defense << std::endl;
    std::cout << "📊 Experience: " << experience << "/" << experienceToNextLevel << std::endl;
    std::cout << "💰 BDP: " << bdp << std::endl;
}

void Player::displayInventory() const
{
    std::cout << "\n🎒 INVENTORY 🎒" << std::endl;
    std::cout << "==============" << std::endl;

    if (inventory.empty())
    {
        std::cout << "Your inventory is empty!" << std::endl;
        return;
    }

    // Group items by name and display count
    for (const auto &pair : itemCounts)
    {
        const std::string &itemName = pair.first;
        int count = pair.second;

        // Find the item to get its emoji
        auto it = std::find_if(inventory.begin(), inventory.end(),
                               [&itemName](const Item &item)
                               { return item.name == itemName; });

        if (it != inventory.end())
        {
            std::cout << it->emoji << " " << itemName << " x" << count << std::endl;
            std::cout << "   " << it->description << std::endl;
        }
    }
}