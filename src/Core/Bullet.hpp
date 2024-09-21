#ifndef BULLET_HPP
#define BULLET_HPP

#include <cmath>

#include "../ASCII_Engine/Sprite.hpp"
#include "../ASCII_Engine/ObjetoDeJogo.hpp"
#include "../ASCII_Engine/SpriteBuffer.hpp"
#include "Entity.hpp"
#include "Directions.hpp"

// Desenho da bala: •

using namespace std;

class Bullet : public Entity {
    private:
        int dist, countMove, MAXMOVE;
        Entity* entity;

    public:
        Bullet(Entity* entity, int damage = 10, int maxMove = 10, int distance = 0) 
            : Entity(ObjetoDeJogo("Bullet", TextSprite("*"), 0, 0), 0, (entity->getDamage() + damage), 0),
            countMove(0), entity(entity),
            MAXMOVE(maxMove), dist(distance) {
            
            this->desativarObj();

            this->setDirection(entity->getDirection());

            int entityL = entity->getSprite()->getLarguraMax();
            int entityA = entity->getSprite()->getAltura();

            int midL = entityL / 2;
            int midA = entityA / 2;

            int entityCenterL = entity->getPosL() + midA;
            int entityCenterC = entity->getPosC() + midL;

            moveTo(entityCenterL, entityCenterC);

            switch (direction) {
                case Directions::UP:
                    this->moveUp(midA + dist);
                    break;

                case Directions::DOWN:
                    if (entityA % 2 == 0) {
                        this->moveDown((entityA - midA) + dist);
                    } else {
                        this->moveDown(midA + dist);
                    }
                    break;

                case Directions::LEFT:
                    this->moveLeft(midL + dist);
                    break;

                case Directions::RIGHT:
                    if (entityL % 2 == 0) {
                        this->moveRight((entityL - midL) + dist);
                    } else {
                        this->moveRight(midL + dist);
                    }
                    break;
            }
        }

        virtual ~Bullet() {}

        bool moveUpdate(SpriteBuffer &screen) {
            if (countMove < MAXMOVE) {
                switch (direction) {
                    case Directions::UP:
                        if (this->getPosL() > 2){
                            this->ativarObj();
                            this->moveUp(1);
                        }
                        break;
                    case Directions::DOWN:
                        if (this->getPosL() < screen.getAltura() - 11){
                            this->ativarObj();
                            this->moveDown(1);
                        }
                        break;
                    case Directions::LEFT:
                        if (this->getPosC() > 3){
                            this->ativarObj();
                            this->moveLeft(1);
                        }
                        break;
                    case Directions::RIGHT:
                        if (this->getPosC() < screen.getLargura(0) - 4) {
                            this->ativarObj();
                            this->moveRight(1);
                        }
                        break;
                }
                countMove++;
                return true;
            }

            return false;
        }
};

#endif // BULLET_HPP