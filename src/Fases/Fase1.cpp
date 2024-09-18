#include "Fase1.hpp"
#include <iostream>
#include "../Core/CaptureKey.hpp"

void FaseUm::init() {
    // Objetos de Jogo
    selectionItem = new ObjetoDeJogo("SelectionItem", Sprite("rsc/Sprites/HudItem.img"), 30, 5);
    selectionItem->desativarObj();
    objs.push_back(selectionItem);

    colisionDoor = new ObjetoDeJogo("colissionDoor", SpriteBuffer(8, 1), 12, 20);
    objs.push_back(colisionDoor);
    
    door = new Door(ObjetoDeJogo("Door", Sprite("rsc/Sprites/Door.img"), 10, 20));
    objs.push_back(door);

    // Items
    Item* key = new Item(ObjetoDeJogo("Key1", Sprite("rsc/Sprites/Key.img"), 20, 20), true, 1);
    key->setUseFunction([this, key]() -> bool{
        if (key->getHolder()->colideCom(*door)) {
            door->setOpened(true);
            return true;
        }
        return false;
    });
    items.push_back(key);

    Item* slingshot = new Item(ObjetoDeJogo("Slingshot", Sprite("rsc/Sprites/Slingshot.img"), 20, 50));
    slingshot->setUseFunction([this, slingshot]() -> bool{
        Bullet* bullet = new Bullet(slingshot->getHolder(), 10, 10, 1);
        bullets.push_back(bullet);
        return true;
    });
    items.push_back(slingshot);

    // Entidades
    player = new Player(ObjetoDeJogo("Player", Sprite("rsc/Sprites/Player.img"), 4, 6));
    entities.push_back(player);

    Entity* enemy = new Entity(ObjetoDeJogo("Enemy", Sprite("rsc/Sprites/Player.img"), 10, 10), 100, 100, 100);
    entities.push_back(enemy);
}

unsigned FaseUm::run(SpriteBuffer &screen) {
    draw(screen);
    system("clear");
    show(screen);

    while (true) {
        char ch = captureKey();

        if (ch == 'q') {
            break;
        }

        int posL = player->getPosL(), posC = player->getPosC();
        Item* item = nullptr;

        switch (ch) {
            case 'w':
                if (posL > 2) {
                    player->moveUp(1);
                    player->setDirection(Directions::UP);
                }
                break;

            case 's':
                if (posL < screen.getAltura() - player->getSprite()->getAltura() - 9) {
                    player->moveDown(1);
                    player->setDirection(Directions::DOWN);
                }
                break;

            case 'a':
                if (posC > 3) {
                    player->moveLeft(1);
                    player->setDirection(Directions::LEFT);
                }
                break;

            case 'd':
                if (posC < screen.getLargura(0) - player->getSprite()->getLarguraMax() - 3) {
                    player->moveRight(1);
                    player->setDirection(Directions::RIGHT);
                }
                break;

            case 'j':
                item = getColissionItem();
                if (item != nullptr) {
                    player->addItem(item);
                }
                break;

            case 'k':
                player->removeItem();
                break;

            case 'l':
                player->useItem();
                break;
        }

        if (colissionObjs()) {
            player->moveTo(posL, posC);
        }

        if (ch == '1' || ch == '2' || ch == '3' || ch == '4' || ch == '5') {
            int index = ch - '1';

            if (player->isSelectedItem(index)) {
                player->deselectItem();
                selectionItem->desativarObj();
            } else {
                player->selectItem(index);
                selectionItem->moveTo(30, 5 + index * 13);
                selectionItem->ativarObj();
            }
        }

        if (player->getHealth() <= 0) {
            return 0;
        }

        update();
        bulletUpdate(screen);
        itemsUpdate();
        entitiesUpdate();
        draw(screen);
        system("clear");
        show(screen);
    }

    return 0;
}

void FaseUm::entitiesUpdate() {
    for (auto entity = entities.begin(); entity != entities.end(); ) {
        if ((*entity)->getHealth() <= 0) {
            Entity* deadEntity = *entity;
            entity = entities.erase(entity);
            delete deadEntity;
        } else {
            ++entity;
        }
    }
}

void FaseUm::itemsUpdate() {
    for (auto item = items.begin(); item != items.end(); ) {
        Item* i = *item;
        if (i->getRemainingUses() == 0) {
            item = items.erase(item);
            delete i;
        } else {
            ++item;
        }
    }
}

void FaseUm::bulletUpdate(SpriteBuffer &screen) {
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
            if ((*entity)->colideCom(**bullet)) {
                (*bullet)->attack(*entity);

                Bullet* hitBullet = *bullet;
                bullet = bullets.erase(bullet);
                delete hitBullet;
                break;
            }
        }
    }
}

bool FaseUm::colissionObjs() const {
    for (auto obj = colisoes.begin(); obj != colisoes.end(); ++obj) {
        if (player->colideCom(**obj)) {
            return true;
        }
    }

    if (player->colideCom(*colisionDoor) && !door->isOpened()) {
        return true;
    }

    return false;
}

Item* FaseUm::getColissionItem() const {
    for (auto item = items.begin(); item != items.end(); ++item)
        if (player->colideCom(**item)) {
            return *item;
        }
    return nullptr;
}

void FaseUm::draw(SpriteBase &screen, int x, int y) {
    background->draw(screen, 0, 0);

    for (auto bullet : bullets) {
        bullet->draw(screen, bullet->getPosL(), bullet->getPosC());
    }

    for (auto obj : objs) {
        obj->draw(screen, obj->getPosL(), obj->getPosC());
    }

    for (auto item : items) {
        item->draw(screen, item->getPosL(), item->getPosC());
    }

    for (auto entity : entities) {
        entity->draw(screen, entity->getPosL(), entity->getPosC());
    }
}