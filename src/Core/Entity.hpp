#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "../ASCII_Engine/ObjetoDeJogo.hpp"
#include "Directions.hpp"
#include <random>

class Entity : public ObjetoDeJogo {
    protected:
        int health, damage, defense;
        Directions direction;
    
    public:
        Entity(const ObjetoDeJogo &obj, int life, int damage, int defense) 
            : ObjetoDeJogo(obj), health(life), damage(damage), defense(defense) {}

        virtual ~Entity() {}

        virtual void attack(Entity* entity) {
            entity->defend(damage);
        }

        virtual void defend(int damage) {
            // Calcula dano mitigado
            int mitigatedDamage = static_cast<int>(damage * (1 - 0.1 * defense));

            // Diminuir a vida com base no dano (mitigado)
            health -= mitigatedDamage;

            // Gerar um valor aleatório entre 1% e 6% da defesa atual
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(1, 6);
            int defenseLossPercentage = dis(gen);

            // Calcular a perda de defesa
            int defenseLoss = static_cast<int>(defense * (defenseLossPercentage / 100.0));

            // Diminuir a defesa
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
};

#endif