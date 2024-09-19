#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "../ASCII_Engine/Core/SpriteBase.hpp"
#include "../ASCII_Engine/ObjetoDeJogo.hpp"
#include "Directions.hpp"
#include "Entity.hpp"
#include "Bullet.hpp"
#include "Player.hpp"
#include <chrono>
#include <vector>
#include <list>

using namespace std;

class Controller {
    private:
        Player* player;
        chrono::steady_clock::time_point lastUseTime;
        int cooldownDurationMs;
        Bullet* newBullet;
        list<Bullet*> bullets;
        Entity* newEntity;
        list<Entity*> entities;
        Map* map;
    
    public:
        Controller(Player* player, Map* map) : player(player), map(map) {
            cooldownDurationMs = 300;
            newBullet = nullptr;
            newEntity = nullptr;
            insertEntity(player);
        }

        virtual ~Controller() {
            for (auto bullet = bullets.begin(); bullet != bullets.end(); ++bullet) {
                delete *bullet;
            }

            for (auto entity = entities.begin(); entity != entities.end(); ++entity) {
                delete *entity;
            }
        }

        void bulletUpdateColission() {
            for (auto bullet = bullets.begin(); bullet != bullets.end(); ) {
                Bullet* b = *bullet;
                if (b->getHealth() <= 0) {
                    Bullet* deadBullet = *bullet;
                    bullet = bullets.erase(bullet);
                    delete deadBullet;
                } else {
                    ++bullet;
                }
            }
        }

        void createBullet(Entity* entity, const int& damage, const int& maxMove, const int& distance) {
            newBullet = new Bullet(entity, damage, maxMove, distance);
            bullets.push_back(newBullet);
        }

        void createEntity(const ObjetoDeJogo &obj, const int& life, const int& damage, const int& defense) {
            newEntity = new Entity(obj, life, damage, defense);
            entities.push_back(newEntity);
        }

        void insertEntity(Entity* entity) {
            entities.push_back(entity);
        }

        void insertBullet(Bullet* bullet) {
            bullets.push_back(bullet);
        }

        void removeEntity(Entity* entity) {
            for (auto it = entities.begin(); it != entities.end(); ++it) {
                if (*it == entity) {
                    entities.erase(it);
                    break;
                }
            }
        }

        void shootAt(Entity* entity, Directions direction) {
            auto now = std::chrono::steady_clock::now();
            auto timeSinceLastUse = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUseTime).count();

            if (timeSinceLastUse >= cooldownDurationMs) {
                entity->setDirection(direction);
                createBullet(entity, 10, 20, 1);
                lastUseTime = now;
            }
        }

        bool checkCollisions() {
            for (auto entity1 = entities.begin(); entity1 != entities.end(); ++entity1)
                for (auto entity2 = entities.begin(); entity2 != entities.end(); ++entity2)
                    if (entity1 != entity2 && (*entity1)->colideCom(**entity2))
                        return true;
            return false;
        }

        void behavior(Entity* entity, Entity* alvo) {
            int rangeVisionA = entity->getRangeVision();
            int rangeVisionL = entity->getRangeVision() * 2;
            int rangeShotA = entity->getRangeShot();
            int rangeShotL = entity->getRangeShot() * 2;

            int distanceL = abs(entity->getCenterL() - alvo->getCenterL());
            int distanceC = abs(entity->getCenterC() - alvo->getCenterC());

            bool isWithinVision = distanceL <= rangeVisionA && distanceC <= rangeVisionL;
            bool isAtVisionEdge = distanceL == rangeVisionA && distanceC == rangeVisionL;
            bool isWithinShotRange = distanceL <= rangeShotA && distanceC <= rangeShotL;

            if (isWithinVision) {
                if (isAtVisionEdge) {
                    handleEdgeBehavior(entity, alvo);
                } else if (isWithinShotRange) {
                    handleShotBehavior(entity, alvo, distanceL, distanceC);
                } else {
                    moveEntityTowardsTarget(entity, alvo, distanceL, distanceC, rangeVisionA, rangeVisionL);
                }
            }
        }

        void handleEdgeBehavior(Entity* entity, Entity* alvo) {
            if (entity->getCenterL() == alvo->getCenterL()) {
                shootAt(entity, entity->getCenterC() < alvo->getCenterC() ? Directions::RIGHT : Directions::LEFT);
            } else if (entity->getCenterC() == alvo->getCenterC()) {
                shootAt(entity, entity->getCenterL() < alvo->getCenterL() ? Directions::DOWN : Directions::UP);
            }
        }

        void handleShotBehavior(Entity* entity, Entity* alvo, int distanceL, int distanceC) {
            int alturaAlvo = alvo->getSprite()->getAltura();
            int larguraAlvo = alvo->getSprite()->getLarguraMax();

            bool inSameRowRange = entity->getCenterL() >= alvo->getPosL() && entity->getCenterL() <= alvo->getPosL() + alturaAlvo;
            bool inSameColumnRange = entity->getCenterC() >= alvo->getPosC() && entity->getCenterC() <= alvo->getPosC() + larguraAlvo;

            if (inSameRowRange) {
                shootAt(entity, entity->getCenterC() < alvo->getCenterC() ? Directions::RIGHT : Directions::LEFT);
            } else if (inSameColumnRange) {
                shootAt(entity, entity->getCenterL() < alvo->getCenterL() ? Directions::DOWN : Directions::UP);
            }
        }

        void moveEntityTowardsTarget(Entity* entity, Entity* alvo, int distanceL, int distanceC, int rangeVisionA, int rangeVisionL) {
            if (distanceL != rangeVisionA) {
                if (entity->getCenterL() < alvo->getCenterL()) {
                    entity->moveDown(1);
                } else if (entity->getCenterL() > alvo->getCenterL()) {
                    entity->moveUp(1);
                }
            }

            if (distanceC != rangeVisionL) {
                if (entity->getCenterC() < alvo->getCenterC()) {
                    entity->moveRight(1);
                } else if (entity->getCenterC() > alvo->getCenterC()) {
                    entity->moveLeft(1);
                }
            }
        }

        Directions findAlternativeRoute(Entity* entity) {
            static std::vector<Directions> possibleDirections = {Directions::UP, Directions::DOWN, Directions::LEFT, Directions::RIGHT};

            for (Directions dir : possibleDirections) {
                entity->setDirection(dir);
                int originalL = entity->getPosL();
                int originalC = entity->getPosC();
                
                switch (dir) {
                    case Directions::UP:
                        entity->moveUp(1);
                        break;
                    case Directions::DOWN:
                        entity->moveDown(1);
                        break;
                    case Directions::LEFT:
                        entity->moveLeft(1);
                        break;
                    case Directions::RIGHT:
                        entity->moveRight(1);
                        break;
                    default:
                        break;
                }

                if (!checkCollisions()) {
                    return dir;
                }

                entity->moveTo(originalL, originalC);
            }

            return Directions::IDLE;
        }


        void update(SpriteBuffer &screen) {
            for (auto it = bullets.begin(); it != bullets.end(); ) {
                Bullet* bullet = *it;
                bool isMoving = bullet->moveUpdate(screen);

                if (!isMoving) {
                    it = bullets.erase(it);
                    delete bullet;
                } else {
                    ++it;
                }
            }

            for (auto bullet = bullets.begin(); bullet != bullets.end(); ++bullet) {
                for (auto entity = entities.begin(); entity != entities.end(); ++entity) {
                    if ((*bullet)->colideCom(**entity)) {
                        (*bullet)->attack(*entity);

                        Bullet* hitBullet = *bullet;
                        bullet = bullets.erase(bullet);
                        delete hitBullet;
                        break;
                    }
                }
            }

            for (auto entity = entities.begin(); entity != entities.end(); ) {
                if ((*entity)->getHealth() <= 0 && (*entity) != player) {
                    Entity* deadEntity = *entity;
                    entity = entities.erase(entity);
                    delete deadEntity;
                } else {
                    Entity* e = *entity;
                    if (e->hasBehavior()) {
                        int posL = e->getPosL(), posC = e->getPosC();

                        if (player != nullptr) {
                            behavior(e, player);

                            if (checkCollisions()) {
                                Directions alternativeDirection = findAlternativeRoute(e);

                                switch (alternativeDirection) {
                                    case Directions::UP:
                                        e->moveUp(1);
                                        break;
                                    case Directions::DOWN:
                                        e->moveDown(1);
                                        break;
                                    case Directions::LEFT:
                                        e->moveLeft(1);
                                        break;
                                    case Directions::RIGHT:
                                        e->moveRight(1);
                                        break;
                                    default:
                                        e->moveTo(posL, posC);
                                        break;
                                }
                            }
                        }
                    }
                    ++entity;
                }
            }
        }

        void draw(SpriteBase &screen) {
            for (auto bullet = bullets.begin(); bullet != bullets.end(); ++bullet) {
                Bullet* b = *bullet;
                b->draw(screen, b->getPosL(), b->getPosC());
            }

            for (auto entity = entities.begin(); entity != entities.end(); ++entity) {
            Entity* e = *entity;
                e->draw(screen, e->getPosL(), e->getPosC());
            }
        }
};

#endif