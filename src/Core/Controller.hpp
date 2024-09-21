#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "../ASCII_Engine/Core/SpriteBase.hpp"
#include "../ASCII_Engine/ObjetoDeJogo.hpp"
#include "Directions.hpp"
#include "Entity.hpp"
#include "Bullet.hpp"
#include "Player.hpp"
#include "Map.hpp"
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
        bool behindWall;

        bool isCollidingWithMap(Entity* entity) {
            return map->colission(entity);
        }
    
    public:
        Controller(Player* player, Map* map, const bool& behindWall = false) : player(player), map(map), behindWall(behindWall) {
            cooldownDurationMs = 300;
            newBullet = nullptr;
            newEntity = nullptr;
            insertEntity(player);
        }

        virtual ~Controller() {
            for (auto bullet : bullets) {
                delete bullet;
            }

            for (auto entity : entities) {
                delete entity;
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
                createBullet(entity, 10, 20, 0);
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

        bool hasLineOfSight(Entity* entity, Entity* target, Map* map) {
            if (behindWall) {
                ObjetoDeJogo tempObj("LineOfSightChecker", SpriteBuffer(1, 1), entity->getPosL(), entity->getPosC());
                Entity* lineChecker = new Entity(tempObj, 1, 0, 0);

                int deltaL = target->getCenterL() - entity->getCenterL();
                int deltaC = target->getCenterC() - entity->getCenterC();
                
                int steps = max(abs(deltaL), abs(deltaC));
                if (steps == 0) {
                    delete lineChecker;
                    return true;
                }

                int stepL = (deltaL * 1000) / steps;
                int stepC = (deltaC * 1000) / steps;

                int currentL = entity->getCenterL() * 1000;
                int currentC = entity->getCenterC() * 1000;

                for (int i = 1; i <= steps; ++i) {
                    currentL += stepL;
                    currentC += stepC;

                    int newL = currentL / 1000;
                    int newC = currentC / 1000;

                    if (newL < 0 || newC < 0 || newL >= map->getMaxL() || newC >= map->getMaxC()) {
                        delete lineChecker;
                        return false;
                    }

                    lineChecker->moveTo(newL, newC);

                    if (map->colission(lineChecker)) {
                        delete lineChecker;
                        return false;
                    }
                }

                delete lineChecker;
                return true;
                
            } else {
                return true;
            }
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

            if (isWithinVision && hasLineOfSight(entity, alvo, map)) {
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

                if (!isCollidingWithMap(entity) && !checkCollisions()) {
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

                if (!isMoving || isCollidingWithMap(bullet) || bullet->getPosL() > screen.getAltura() - bullet->getSprite()->getAltura() - 20) {
                    it = bullets.erase(it);
                    delete bullet;
                } else {
                    ++it;
                }
            }

            for (auto bullet = bullets.begin(); bullet != bullets.end(); ) {
                Bullet* b = *bullet;
                bool bulletRemoved = false;

                for (auto entity = entities.begin(); entity != entities.end(); ++entity) {
                    if (b->colideCom(**entity)) {
                        b->attack(*entity);
                        bulletRemoved = true;
                        break;
                    }
                }

                if (bulletRemoved) {
                    bullet = bullets.erase(bullet);
                    delete b;
                } else {
                    ++bullet;
                }
            }

            for (auto entity1 = entities.begin(); entity1 != entities.end(); ) {
                Entity* e1 = *entity1;

                if (e1->getHealth() <= 0 && e1 != player) {
                    entity1 = entities.erase(entity1);
                    delete e1;
                } else {
                    if (e1->hasBehavior() && player != nullptr) {
                        int posL = e1->getPosL(), posC = e1->getPosC();
                        behavior(e1, player);

                        if (isCollidingWithMap(e1)) {
                            Directions alternativeDirection = findAlternativeRoute(e1);

                            switch (alternativeDirection) {
                                case Directions::UP:
                                    e1->moveUp(1);
                                    break;
                                case Directions::DOWN:
                                    e1->moveDown(1);
                                    break;
                                case Directions::LEFT:
                                    e1->moveLeft(1);
                                    break;
                                case Directions::RIGHT:
                                    e1->moveRight(1);
                                    break;
                                default:
                                    e1->moveTo(posL, posC);
                                    break;
                            }
                        }

                        for (auto entity2 = entities.begin(); entity2 != entities.end(); ) {
                            if (*entity2 != e1 && e1->colideCom(**entity2)) {
                                Directions alternativeDirection = findAlternativeRoute(e1);

                                switch (alternativeDirection) {
                                    case Directions::UP:
                                        e1->moveUp(1);
                                        break;
                                    case Directions::DOWN:
                                        e1->moveDown(1);
                                        break;
                                    case Directions::LEFT:
                                        e1->moveLeft(1);
                                        break;
                                    case Directions::RIGHT:
                                        e1->moveRight(1);
                                        break;
                                    default:
                                        e1->moveTo(posL, posC);
                                        break;
                                }
                            }
                            ++entity2;
                        }
                    }
                    ++entity1;
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