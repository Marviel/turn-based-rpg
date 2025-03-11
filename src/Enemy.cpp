#include "Enemy.h"
#include <iostream>

Enemy::Enemy(const std::string &name, int health, int attack, int defense,
             int experienceReward, int bdpReward, bool isBoss, const std::string &emoji)
    : Entity(name, health, attack, defense),
      experienceReward(experienceReward),
      bdpReward(bdpReward),
      isBoss(isBoss),
      emoji(emoji) {}

int Enemy::getExperienceReward() const
{
    return experienceReward;
}

int Enemy::getBDPReward() const
{
    return bdpReward;
}

bool Enemy::getIsBoss() const
{
    return isBoss;
}

std::string Enemy::getEmoji() const
{
    return emoji;
}

void Enemy::displayStats() const
{
    if (isBoss)
    {
        std::cout << "\n🔥🔥🔥 BOSS 🔥🔥🔥" << std::endl;
    }

    std::cout << emoji << " " << name;
    if (isBoss)
    {
        std::cout << " 👑";
    }
    std::cout << std::endl;

    std::cout << "❤️ Health: " << health << "/" << maxHealth << std::endl;
    std::cout << "⚔️ Attack: " << attack << std::endl;
    std::cout << "🛡️ Defense: " << defense << std::endl;

    if (isBoss)
    {
        std::cout << "💀 DANGER LEVEL: EXTREME 💀" << std::endl;
    }
}