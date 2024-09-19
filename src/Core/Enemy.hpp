#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Entity.hpp"

using namespace std;

class Enemy : public Entity {
    private:
        std::chrono::steady_clock::time_point lastUseTime;
        int cooldownDurationMs;
    public:
        Enemy(const int& posL, const int& posC, const int& vision = 20, const int& bulletRange = 10, const int& life = 100, const int& damage = 10, const int& defense = 100) 
            : Entity(ObjetoDeJogo("Enemy", Sprite("rsc/Sprites/Enemy.img"), posL, posC), life, damage, defense, true, 20, 10), cooldownDurationMs(300) {
                lastUseTime = std::chrono::steady_clock::now() - std::chrono::milliseconds(cooldownDurationMs);
            }

        virtual ~Enemy() {}  
};

#endif // ENEMY_HPP