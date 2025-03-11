#ifndef ENEMY_H
#define ENEMY_H

#include <string>
#include "Entity.h"

class Enemy : public Entity
{
private:
    int experienceReward;
    int bdpReward;
    bool isBoss;
    std::string emoji;

public:
    Enemy(const std::string &name, int health, int attack, int defense,
          int experienceReward, int bdpReward, bool isBoss = false,
          const std::string &emoji = "👹");

    // Getters
    int getExperienceReward() const;
    int getBDPReward() const;
    bool getIsBoss() const;
    std::string getEmoji() const;

    // Override methods from Entity
    void displayStats() const override;
};

#endif // ENEMY_H