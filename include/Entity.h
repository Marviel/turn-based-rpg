#ifndef ENTITY_H
#define ENTITY_H

#include <string>

class Entity
{
protected:
    std::string name;
    int health;
    int maxHealth;
    int attack;
    int defense;

public:
    Entity(const std::string &name, int health, int attack, int defense);
    virtual ~Entity() = default;

    // Getters
    std::string getName() const;
    int getHealth() const;
    int getMaxHealth() const;
    int getAttack() const;
    int getDefense() const;

    // Setters and modifiers
    void setHealth(int health);
    void takeDamage(int damage);
    void heal(int amount);

    // Combat methods
    int calculateDamage() const;
    bool isAlive() const;

    // Display methods
    virtual void displayStats() const;
};

#endif // ENTITY_H