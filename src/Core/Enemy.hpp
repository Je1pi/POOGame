#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Entity.hpp"
#include "../Fases/Fase1.hpp"

using namespace std;

class Enemy : public Entity {
    private:
        int rangeVision, rangeShot;
        FaseUm* fase;
    public:
        Enemy(FaseUm* fase, const int& posL, const int& posC, const int& vision = 20, const int& bulletRange = 10, const int& life = 100, const int& damage = 100, const int& defense = 100) 
            : Entity(ObjetoDeJogo("Enemy", Sprite("rsc/Sprites/Enemy.img"), posL, posC), life, damage, defense), rangeVision(vision), rangeShot(bulletRange), fase(fase) {}

        virtual ~Enemy() {}

        void behavior(Entity* player) override {
            int distanceL = abs(getCenterL() - player->getCenterL());
            int distanceC = abs(getCenterC() - player->getCenterC());

            if (distanceL <= rangeVision && distanceC <= rangeVision) {
                if ((distanceL == 0 || distanceC == 0) && (distanceL <= rangeShot && distanceC <= rangeShot)) {
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
                    if (getCenterL() < player->getCenterL()) {
                        moveDown(1);
                    } else if (getCenterL() > player->getCenterL()) {
                        moveUp(1);
                    }

                    if (getCenterC() < player->getCenterC()) {
                        moveRight(1);
                    } else if (getCenterC() > player->getCenterC()) {
                        moveLeft(1);
                    }
                }
            }
        }

        void shootAt(Directions direction) {
            Bullet* bullet = new Bullet(this, 10, 20, 1);
            bullet->setDirection(direction);
            fase->addBullet(bullet);
        }
};


#endif // ENEMY_HPP