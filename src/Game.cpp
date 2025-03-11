#include "Game.h"
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <random>
#include <limits>
#include <csignal>

#ifdef _WIN32
#include <windows.h>
#else
#include <cstdlib>
#endif

// Global flag for signal handling
volatile sig_atomic_t exitRequested = 0;

// Signal handler function
void signalHandler(int signal)
{
    exitRequested = 1;
    std::cout << "\nExiting game...\n";
}

// Helper function to get valid integer input
int getValidIntInput()
{
    int choice;
    std::string input;
    bool validInput = false;

    while (!validInput)
    {
        std::getline(std::cin, input);

        // Check for EOF (Ctrl+D)
        if (std::cin.eof())
        {
            std::cout << "\nExiting game...\n";
            exit(0);
        }

        try
        {
            if (input.empty())
            {
                std::cout << "Please enter a number: ";
                continue;
            }

            // Check if input is numeric
            for (char c : input)
            {
                if (!std::isdigit(c))
                {
                    throw std::invalid_argument("Not a number");
                }
            }

            choice = std::stoi(input);
            validInput = true;
        }
        catch (const std::exception &)
        {
            std::cout << "Invalid input. Please enter a number: ";
        }
    }

    return choice;
}

Game::Game()
    : currentState(GameState::MAIN_MENU),
      player("Adventurer"),
      currentDungeonLevel(1),
      maxDungeonLevel(5)
{
    initializeGame();

    // Set up signal handlers
    std::signal(SIGINT, signalHandler); // Ctrl+C
}

void Game::initializeGame()
{
    createNPCs();
    createEnemies();
}

void Game::createNPCs()
{
    // Create friendly NPC named Nick
    auto nick = std::make_unique<NPC>("Nick", "👨‍🦰", true);
    nick->addDialogue("Welcome to the dungeon, adventurer! Be careful down there.");
    nick->addDialogue("I've heard rumors of a powerful enemy lurking in the depths...");
    nick->addDialogue("Need supplies? I've got potions and equipment for sale!");
    nick->addDialogue("My evil twin brother NICK is causing trouble again. Can you stop him?");

    // Add shop items
    nick->addShopItem("Health Potion", 20);
    nick->addShopItem("Attack Boost", 50);
    nick->addShopItem("Defense Boost", 50);

    npcs.push_back(std::move(nick));
}

void Game::createEnemies()
{
    // Clear existing enemies
    enemies.clear();

    // Create regular enemies based on dungeon level
    for (int i = 0; i < 3 + currentDungeonLevel; ++i)
    {
        std::string name;
        std::string emoji;
        int health, attack, defense, xpReward, bdpReward;

        // Randomize enemy type
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distr(0, 3);

        switch (distr(gen))
        {
        case 0:
            name = "Goblin";
            emoji = "👺";
            health = 20 + currentDungeonLevel * 5;
            attack = 5 + currentDungeonLevel * 2;
            defense = 2 + currentDungeonLevel;
            xpReward = 15 + currentDungeonLevel * 5;
            bdpReward = 5 + currentDungeonLevel * 2;
            break;
        case 1:
            name = "Skeleton";
            emoji = "💀";
            health = 15 + currentDungeonLevel * 4;
            attack = 7 + currentDungeonLevel * 2;
            defense = 1 + currentDungeonLevel;
            xpReward = 20 + currentDungeonLevel * 5;
            bdpReward = 7 + currentDungeonLevel * 2;
            break;
        case 2:
            name = "Slime";
            emoji = "🟢";
            health = 25 + currentDungeonLevel * 6;
            attack = 4 + currentDungeonLevel;
            defense = 3 + currentDungeonLevel;
            xpReward = 10 + currentDungeonLevel * 4;
            bdpReward = 3 + currentDungeonLevel * 2;
            break;
        case 3:
            name = "Bat";
            emoji = "🦇";
            health = 10 + currentDungeonLevel * 3;
            attack = 6 + currentDungeonLevel * 2;
            defense = 1 + currentDungeonLevel;
            xpReward = 12 + currentDungeonLevel * 4;
            bdpReward = 4 + currentDungeonLevel * 2;
            break;
        }

        enemies.push_back(std::make_unique<Enemy>(
            name, health, attack, defense, xpReward, bdpReward, false, emoji));
    }

    // Add final boss at the last level
    if (currentDungeonLevel == maxDungeonLevel)
    {
        enemies.push_back(std::make_unique<Enemy>(
            "NICK", 200, 25, 15, 500, 1000, true, "😈"));
    }
}

void Game::run()
{
    while (currentState != GameState::GAME_OVER && currentState != GameState::VICTORY)
    {
        // Check if exit was requested via signal
        if (exitRequested)
        {
            std::cout << "Game terminated by user." << std::endl;
            currentState = GameState::GAME_OVER;
            break;
        }

        clearScreen();

        switch (currentState)
        {
        case GameState::MAIN_MENU:
            displayMainMenu();
            break;
        case GameState::EXPLORING:
            handleExploring();
            break;
        case GameState::COMBAT:
            handleCombat();
            break;
        case GameState::SHOP:
            handleShop();
            break;
        case GameState::TALKING_TO_NPC:
            handleNPCInteraction();
            break;
        case GameState::GAME_OVER:
            displayGameOver();
            break;
        case GameState::VICTORY:
            displayVictory();
            break;
        }
    }
}

void Game::setState(GameState newState)
{
    currentState = newState;
}

GameState Game::getState() const
{
    return currentState;
}

void Game::clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void Game::pauseGame()
{
    std::cout << "\nPress Enter to continue...";
    std::string input;
    std::getline(std::cin, input);

    // Check for EOF (Ctrl+D)
    if (std::cin.eof())
    {
        std::cout << "\nExiting game...\n";
        exit(0);
    }
}

void Game::displayMainMenu()
{
    std::cout << "🏰 DUNGEON CRAWLER RPG 🐉" << std::endl;
    std::cout << "=========================" << std::endl;
    std::cout << "\n1. Start New Game" << std::endl;
    std::cout << "2. Exit" << std::endl;
    std::cout << "\nEnter your choice: ";

    int choice = getValidIntInput();

    switch (choice)
    {
    case 1:
        std::cout << "\nWhat is your name, brave adventurer? ";
        {
            std::string playerName;
            std::getline(std::cin, playerName);
            if (!playerName.empty())
            {
                player = Player(playerName);
            }
        }
        std::cout << "\nWelcome, " << player.getName() << "! Your adventure begins..." << std::endl;
        pauseGame();
        setState(GameState::EXPLORING);
        break;
    case 2:
        setState(GameState::GAME_OVER);
        break;
    default:
        std::cout << "\nInvalid choice. Please try again." << std::endl;
        pauseGame();
        break;
    }
}

void Game::displayGameOver()
{
    std::cout << "💀 GAME OVER 💀" << std::endl;
    std::cout << "==============" << std::endl;
    std::cout << "\nYour adventure has come to an end." << std::endl;
    pauseGame();
}

void Game::displayVictory()
{
    std::cout << "🎉 VICTORY! 🎉" << std::endl;
    std::cout << "==============" << std::endl;
    std::cout << "\nCongratulations, " << player.getName() << "!" << std::endl;
    std::cout << "You have defeated NICK and saved the dungeon!" << std::endl;
    std::cout << "\nFinal Stats:" << std::endl;
    player.displayStats();
    pauseGame();
}

void Game::handleExploring()
{
    std::cout << "🧭 EXPLORING DUNGEON - LEVEL " << currentDungeonLevel << " 🧭" << std::endl;
    std::cout << "===============================" << std::endl;

    player.displayStats();

    std::cout << "\nWhat would you like to do?" << std::endl;
    std::cout << "1. Look for enemies" << std::endl;
    std::cout << "2. Talk to Nick" << std::endl;
    std::cout << "3. Rest (restore some health)" << std::endl;
    std::cout << "4. Exit game" << std::endl;

    std::cout << "\nEnter your choice: ";
    int choice = getValidIntInput();

    switch (choice)
    {
    case 1:
    {
        std::cout << "\nLooking for enemies..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // Random chance to find an enemy
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distr(0, enemies.size() - 1);

        int enemyIndex = distr(gen);
        std::cout << "You encountered a " << enemies[enemyIndex]->getEmoji()
                  << " " << enemies[enemyIndex]->getName() << "!" << std::endl;

        pauseGame();
        setState(GameState::COMBAT);
        break;
    }
    case 2:
        std::cout << "\nYou approach Nick..." << std::endl;
        pauseGame();
        setState(GameState::TALKING_TO_NPC);
        break;
    case 3:
    {
        std::cout << "\nYou take a moment to rest..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));

        int healAmount = player.getMaxHealth() / 5; // Heal 20% of max health
        player.heal(healAmount);

        pauseGame();
        break;
    }
    case 4:
        setState(GameState::GAME_OVER);
        break;
    default:
        std::cout << "\nInvalid choice. Please try again." << std::endl;
        pauseGame();
        break;
    }
}

void Game::handleCombat()
{
    // Find a random enemy to fight
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, enemies.size() - 1);

    int enemyIndex = distr(gen);
    Enemy &enemy = *enemies[enemyIndex];

    bool combatEnded = false;

    while (!combatEnded)
    {
        clearScreen();
        std::cout << "⚔️ COMBAT ⚔️" << std::endl;
        std::cout << "===========" << std::endl;

        player.displayStats();
        std::cout << std::endl;
        enemy.displayStats();

        std::cout << "\nWhat would you like to do?" << std::endl;
        std::cout << "1. Attack" << std::endl;
        std::cout << "2. Defend (reduce damage taken)" << std::endl;
        std::cout << "3. Run away" << std::endl;

        std::cout << "\nEnter your choice: ";
        int choice = getValidIntInput();

        switch (choice)
        {
        case 1:
        {
            // Player attacks
            std::cout << "\n"
                      << player.getName() << " attacks " << enemy.getName() << "! ⚔️" << std::endl;
            int damage = player.calculateDamage();
            enemy.takeDamage(damage);

            // Check if enemy is defeated
            if (!enemy.isAlive())
            {
                std::cout << "\nYou defeated the " << enemy.getEmoji() << " " << enemy.getName() << "! 🎉" << std::endl;

                // Gain rewards
                player.gainExperience(enemy.getExperienceReward());
                player.earnBDP(enemy.getBDPReward());

                // Check if it was the final boss
                if (enemy.getIsBoss())
                {
                    std::cout << "\n🎊 You have defeated the final boss, NICK! 🎊" << std::endl;
                    pauseGame();
                    setState(GameState::VICTORY);
                    return;
                }

                // Move to next dungeon level if all enemies are defeated
                if (currentDungeonLevel < maxDungeonLevel)
                {
                    std::cout << "\nYou've cleared this area! Moving to dungeon level "
                              << (currentDungeonLevel + 1) << "..." << std::endl;
                    currentDungeonLevel++;
                    createEnemies(); // Generate new enemies for the next level
                }

                pauseGame();
                setState(GameState::EXPLORING);
                combatEnded = true;
                break;
            }

            // Enemy attacks
            std::cout << "\n"
                      << enemy.getEmoji() << " " << enemy.getName() << " attacks you! ⚔️" << std::endl;
            damage = enemy.calculateDamage();
            player.takeDamage(damage);

            // Check if player is defeated
            if (!player.isAlive())
            {
                std::cout << "\nYou have been defeated! 💀" << std::endl;
                pauseGame();
                setState(GameState::GAME_OVER);
                combatEnded = true;
                break;
            }

            pauseGame();
            break;
        }
        case 2:
        {
            // Player defends (temporarily increase defense)
            std::cout << "\n"
                      << player.getName() << " takes a defensive stance! 🛡️" << std::endl;
            int tempDefenseBoost = 5;
            int originalDefense = player.getDefense();

            // Enemy attacks with reduced damage
            std::cout << "\n"
                      << enemy.getEmoji() << " " << enemy.getName() << " attacks you! ⚔️" << std::endl;
            int damage = enemy.calculateDamage() - tempDefenseBoost;
            if (damage < 1)
                damage = 1;

            player.takeDamage(damage);

            // Check if player is defeated
            if (!player.isAlive())
            {
                std::cout << "\nYou have been defeated! 💀" << std::endl;
                pauseGame();
                setState(GameState::GAME_OVER);
                combatEnded = true;
                break;
            }

            pauseGame();
            break;
        }
        case 3:
        {
            // Attempt to run away
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distr(1, 10);

            int escapeChance = distr(gen);

            if (escapeChance > 3 || enemy.getIsBoss())
            { // 70% chance to escape, can't escape from boss
                std::cout << "\nYou successfully escaped! 🏃‍♂️💨" << std::endl;
                pauseGame();
                setState(GameState::EXPLORING);
                combatEnded = true;
            }
            else
            {
                std::cout << "\nYou failed to escape! 😱" << std::endl;

                // Enemy gets a free attack
                std::cout << "\n"
                          << enemy.getEmoji() << " " << enemy.getName() << " attacks you! ⚔️" << std::endl;
                int damage = enemy.calculateDamage();
                player.takeDamage(damage);

                // Check if player is defeated
                if (!player.isAlive())
                {
                    std::cout << "\nYou have been defeated! 💀" << std::endl;
                    pauseGame();
                    setState(GameState::GAME_OVER);
                    combatEnded = true;
                    break;
                }

                pauseGame();
            }
            break;
        }
        default:
            std::cout << "\nInvalid choice. Please try again." << std::endl;
            pauseGame();
            break;
        }
    }
}

void Game::handleShop()
{
    clearScreen();
    std::cout << "🛒 SHOP 🛒" << std::endl;
    std::cout << "=========" << std::endl;

    // Find Nick (the shopkeeper)
    NPC *shopkeeper = nullptr;
    for (const auto &npc : npcs)
    {
        if (npc->getName() == "Nick" && npc->getIsShopkeeper())
        {
            shopkeeper = npc.get();
            break;
        }
    }

    if (!shopkeeper)
    {
        std::cout << "Shop is currently closed. Please come back later." << std::endl;
        pauseGame();
        setState(GameState::EXPLORING);
        return;
    }

    std::cout << shopkeeper->getEmoji() << " " << shopkeeper->getName() << ": \"Welcome to my shop!\"" << std::endl;

    player.displayStats();
    std::cout << "\nAvailable Items:" << std::endl;

    const auto &shopItems = shopkeeper->getShopItems();
    int itemIndex = 1;
    for (const auto &item : shopItems)
    {
        std::cout << itemIndex << ". " << item.first << " - " << item.second << " BDP" << std::endl;
        itemIndex++;
    }

    std::cout << itemIndex << ". Exit Shop" << std::endl;

    std::cout << "\nEnter your choice: ";
    int choice = getValidIntInput();

    if (choice == itemIndex)
    {
        // Exit shop
        setState(GameState::EXPLORING);
        return;
    }

    if (choice < 1 || choice > shopItems.size())
    {
        std::cout << "\nInvalid choice. Please try again." << std::endl;
        pauseGame();
        return;
    }

    // Find the selected item
    auto it = shopItems.begin();
    std::advance(it, choice - 1);

    std::string itemName = it->first;
    int itemPrice = it->second;

    // Check if player has enough BDP
    if (player.getBDP() < itemPrice)
    {
        std::cout << "\nYou don't have enough BDP to buy this item! 😢" << std::endl;
        pauseGame();
        return;
    }

    // Purchase the item
    player.spendBDP(itemPrice);

    // Apply item effects
    if (itemName == "Health Potion")
    {
        int healAmount = player.getMaxHealth() / 2; // Heal 50% of max health
        player.heal(healAmount);
    }
    else if (itemName == "Attack Boost")
    {
        // Permanently increase attack
        player = Player(player.getName());
        player.gainExperience(50); // Give some XP to potentially level up
        std::cout << "\nYour attack power has increased! 💪" << std::endl;
    }
    else if (itemName == "Defense Boost")
    {
        // Permanently increase defense
        player = Player(player.getName());
        player.gainExperience(50); // Give some XP to potentially level up
        std::cout << "\nYour defense has increased! 🛡️" << std::endl;
    }

    std::cout << "\nThank you for your purchase!" << std::endl;
    pauseGame();
}

void Game::handleNPCInteraction()
{
    clearScreen();
    std::cout << "💬 TALKING TO NPC 💬" << std::endl;
    std::cout << "===================" << std::endl;

    // Find Nick
    NPC *nick = nullptr;
    for (const auto &npc : npcs)
    {
        if (npc->getName() == "Nick")
        {
            nick = npc.get();
            break;
        }
    }

    if (!nick)
    {
        std::cout << "Nick is not available right now." << std::endl;
        pauseGame();
        setState(GameState::EXPLORING);
        return;
    }

    nick->displayInfo();

    std::cout << "\nWhat would you like to do?" << std::endl;
    std::cout << "1. Talk to " << nick->getName() << std::endl;
    std::cout << "2. Shop" << std::endl;
    std::cout << "3. Leave" << std::endl;

    std::cout << "\nEnter your choice: ";
    int choice = getValidIntInput();

    switch (choice)
    {
    case 1:
        std::cout << "\n"
                  << nick->getEmoji() << " " << nick->getName() << ": \""
                  << nick->getRandomDialogue() << "\"" << std::endl;
        pauseGame();
        break;
    case 2:
        if (nick->getIsShopkeeper())
        {
            setState(GameState::SHOP);
        }
        else
        {
            std::cout << "\n"
                      << nick->getName() << " doesn't have anything to sell." << std::endl;
            pauseGame();
        }
        break;
    case 3:
        setState(GameState::EXPLORING);
        break;
    default:
        std::cout << "\nInvalid choice. Please try again." << std::endl;
        pauseGame();
        break;
    }
}