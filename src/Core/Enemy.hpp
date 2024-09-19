#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Entity.hpp"

using namespace std;

class Enemy : public Entity {
    private:
        int rangeVisionA, rangeVisionL, rangeShotA, rangeShotL;
        std::chrono::steady_clock::time_point lastUseTime;
        int cooldownDurationMs;
        list<Bullet*> &bullets;
    public:
        Enemy(list<Bullet*> &bulletsList, const int& posL, const int& posC, const int& vision = 20, const int& bulletRange = 10, const int& life = 100, const int& damage = 10, const int& defense = 100) 
            : Entity(ObjetoDeJogo("Enemy", Sprite("rsc/Sprites/Enemy.img"), posL, posC), life, damage, defense), bullets(bulletsList), cooldownDurationMs(300) {
                rangeVisionA = vision;
                rangeVisionL = vision*2;
                rangeShotA = bulletRange;
                rangeShotL = bulletRange*2;
                lastUseTime = std::chrono::steady_clock::now() - std::chrono::milliseconds(cooldownDurationMs);
            }

        virtual ~Enemy() {}

        void behavior(Entity* player) override {
            int distanceL = abs(getCenterL() - player->getCenterL());
            int distanceC = abs(getCenterC() - player->getCenterC());

            if (distanceL <= rangeVisionA && distanceC <= rangeVisionL) {
                if (distanceL == rangeVisionA && distanceC == rangeVisionL) {
                    if (distanceL == 0) {
                        if (getCenterC() < player->getCenterC()) {
                            shootAt(Directions::RIGHT);
                        } else {
                            shootAt(Directions::LEFT);
                        }
                    } else if (distanceC == 0) {
                        if (getCenterL() < player->getCenterL()) {
                            shootAt(Directions::DOWN);
                        } else {
                            shootAt(Directions::UP);
                        }
                    }
                } else if ((distanceL == 0 || distanceC == 0) && (distanceL <= rangeShotA && distanceC <= rangeShotL)) {
                    if (distanceL == 0) {
                        if (getCenterC() < player->getCenterC()) {
                            shootAt(Directions::RIGHT);
                        } else {
                            shootAt(Directions::LEFT);
                        }
                    } else if (distanceC == 0) {
                        if (getCenterL() < player->getCenterL()) {
                            shootAt(Directions::DOWN);
                        } else {
                            shootAt(Directions::UP);
                        }
                    }
                } else {
                    if (distanceL != rangeVisionA) {
                        if (getCenterL() < player->getCenterL()) {
                            moveDown(1);
                        } else if (getCenterL() > player->getCenterL()) {
                            moveUp(1);
                        }
                    }

                    if (distanceC != rangeVisionL) {
                        if (getCenterC() < player->getCenterC()) {
                            moveRight(1);
                        } else if (getCenterC() > player->getCenterC()) {
                            moveLeft(1);
                        }
                    }
                }
            }
        }

        void shootAt(Directions direction) {
            auto now = std::chrono::steady_clock::now();
            auto timeSinceLastUse = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUseTime).count();

            if (timeSinceLastUse >= cooldownDurationMs) {
                this->setDirection(direction);
                Bullet* bullet = new Bullet(this, 10, 20, 1);
                bullets.push_back(bullet);
                lastUseTime = now;
            }
        }
        
};

#endif // ENEMY_HPP