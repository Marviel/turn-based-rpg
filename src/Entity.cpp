#include "Entity.h"
#include <iostream>
#include <random>

Entity::Entity(const std::string &name, int health, int attack, int defense)
    : name(name), health(health), maxHealth(health), attack(attack), defense(defense) {}

std::string Entity::getName() const
{
    return name;
}

int Entity::getHealth() const
{
    return health;
}

int Entity::getMaxHealth() const
{
    return maxHealth;
}

int Entity::getAttack() const
{
    return attack;
}

int Entity::getDefense() const
{
    return defense;
}

void Entity::setHealth(int newHealth)
{
    health = (newHealth > maxHealth) ? maxHealth : newHealth;
}

void Entity::takeDamage(int damage)
{
    int actualDamage = damage - defense;
    if (actualDamage < 1)
        actualDamage = 1; // Minimum damage is 1

    health -= actualDamage;
    if (health < 0)
        health = 0;

    std::cout << name << " takes " << actualDamage << " damage! 💥" << std::endl;
}

void Entity::heal(int amount)
{
    health += amount;
    if (health > maxHealth)
        health = maxHealth;

    std::cout << name << " heals for " << amount << " health! ❤️" << std::endl;
}

int Entity::calculateDamage() const
{
    // Add some randomness to damage
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(-2, 2); // Random modifier between -2 and +2

    int damage = attack + distr(gen);
    return (damage < 1) ? 1 : damage; // Minimum damage is 1
}

bool Entity::isAlive() const
{
    return health > 0;
}

void Entity::displayStats() const
{
    std::cout << name << " - Health: " << health << "/" << maxHealth
              << " | Attack: " << attack << " | Defense: " << defense << std::endl;
}