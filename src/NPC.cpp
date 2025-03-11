#include "NPC.h"
#include <iostream>
#include <random>

NPC::NPC(const std::string &name, const std::string &emoji, bool isShopkeeper)
    : name(name), emoji(emoji), isShopkeeper(isShopkeeper) {}

std::string NPC::getName() const
{
    return name;
}

std::string NPC::getEmoji() const
{
    return emoji;
}

bool NPC::getIsShopkeeper() const
{
    return isShopkeeper;
}

void NPC::addDialogue(const std::string &dialogue)
{
    dialogues.push_back(dialogue);
}

std::string NPC::getRandomDialogue() const
{
    if (dialogues.empty())
    {
        return "...";
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, dialogues.size() - 1);

    return dialogues[distr(gen)];
}

void NPC::addShopItem(const std::string &itemName, int price)
{
    shopItems[itemName] = price;
}

const std::map<std::string, int> &NPC::getShopItems() const
{
    return shopItems;
}

void NPC::displayInfo() const
{
    std::cout << emoji << " " << name;

    if (isShopkeeper)
    {
        std::cout << " 🛒";
    }

    std::cout << std::endl;

    // Display a random dialogue
    std::cout << "\"" << getRandomDialogue() << "\"" << std::endl;

    // If shopkeeper, display shop items
    if (isShopkeeper && !shopItems.empty())
    {
        std::cout << "\n🛒 Shop Items:" << std::endl;
        for (const auto &item : shopItems)
        {
            std::cout << "  - " << item.first << ": " << item.second << " BDP" << std::endl;
        }
    }
}