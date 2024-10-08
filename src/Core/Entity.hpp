#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "../ASCII_Engine/ObjetoDeJogo.hpp"
#include "Directions.hpp"
#include <random>
#include <list>

using namespace std;

class Entity : public ObjetoDeJogo {
    protected:
        int health, damage, defense, rangeVision, rangeShot;
        bool behavior;
        Directions direction;

    public:
        Entity(const ObjetoDeJogo &obj, const int& life, const int& damage, const int& defense, const bool& behavior = false, const int& rangeVision = 0, const int& rangeShot = 0) 
            : ObjetoDeJogo(obj), health(life), damage(damage), 
            defense(defense), behavior(behavior), rangeVision(rangeVision),
            rangeShot(rangeShot) {}

        virtual ~Entity() {}

        virtual void attack(Entity* entity) {
            entity->defend(damage);
        }

        virtual void defend(int damage) {
            double defenseFactor = 0.1 * defense;
            int mitigatedDamage = damage - defenseFactor;

            if (mitigatedDamage < 0) {
                mitigatedDamage = 0;
            }

            health -= mitigatedDamage;

            if (health < 0) {
                health = 0;
            }

            static std::mt19937 gen(std::random_device{}());
            std::uniform_int_distribution<> dis(1, 100);
            int numberRandom = dis(gen);
            int defenseLossPercentage = 0;

            switch (numberRandom) {
                case 1 ... 10:
                    defenseLossPercentage = 1;
                    break;
                case 11 ... 25:
                    defenseLossPercentage = 5;
                    break;
                case 26 ... 44:
                    defenseLossPercentage = 10;
                    break;
                case 45 ... 68:
                    defenseLossPercentage = 20;
                    break;
                case 69 ... 90:
                    defenseLossPercentage = 30;
                    break;
            }

            int defenseLoss = (defense * (defenseLossPercentage / 100.0));
            defense -= defenseLoss;
        }

        void setDirection(Directions direction) {
            this->direction = direction;
        }

        Directions getDirection() const {
            return direction;
        }

        int getHealth() const {
            return health;
        }

        int getDamage() const {
            return damage;
        }

        int getDefense() const {
            return defense;
        }

        int getCenterL() const {
            return this->getPosL() + (this->getSprite()->getAltura() / 2);
        }

        int getCenterC() const {
            return this->getPosC() + (this->getSprite()->getLarguraMax() / 2);
        }

        int getRangeVision() const {
            return rangeVision;
        }

        int getRangeShot() const {
            return rangeShot;
        }

        bool hasBehavior() const {
            return behavior;
        }
};

#endif