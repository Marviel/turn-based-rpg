#ifndef GAME_H
#define GAME_H

#include <vector>
#include <memory>
#include <string>
#include "Player.h"
#include "Enemy.h"
#include "NPC.h"

enum class GameState
{
    MAIN_MENU,
    EXPLORING,
    COMBAT,
    SHOP,
    TALKING_TO_NPC,
    GAME_OVER,
    VICTORY
};

class Game
{
private:
    GameState currentState;
    Player player;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<std::unique_ptr<NPC>> npcs;
    int currentDungeonLevel;
    int maxDungeonLevel;

    // Private methods
    void initializeGame();
    void createNPCs();
    void createEnemies();
    void displayMainMenu();
    void displayGameOver();
    void displayVictory();
    void handleCombat();
    void handleExploring();
    void handleShop();
    void handleNPCInteraction();
    void levelUp();
    void generateDungeon();

public:
    Game();
    void run();
    void setState(GameState newState);
    GameState getState() const;
    void clearScreen();
    void pauseGame();
};

#endif // GAME_H