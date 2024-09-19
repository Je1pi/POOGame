#include "Fase1.hpp"
#include <iostream>
#include "../Core/CaptureKey.hpp"

void FaseUm::init() {
    // Objetos de Jogo
    selectionItem = new ObjetoDeJogo("SelectionItem", Sprite("rsc/Sprites/SelectedItem.img"), 30, 5);
    selectionItem->desativarObj();
    objs.push_back(selectionItem);

    colisionDoor = new ObjetoDeJogo("colissionDoor", SpriteBuffer(8, 1), 12, 200);
    objs.push_back(colisionDoor);
    
    door = new Door(ObjetoDeJogo("Door", Sprite("rsc/Sprites/Door.img"), 10, 200));
    objs.push_back(door);

    // Entidades
    player = new Player(10, 10);
    controller = new Controller(player);
    
    Enemy* enemy1 = new Enemy(30, 80);
    controller->insertEntity(enemy1);

    Enemy* enemy2 = new Enemy(30, 100);
    controller->insertEntity(enemy2);

    Enemy* enemy3 = new Enemy(30, 120);
    controller->insertEntity(enemy3);

    // Items
    Item* key = new Item(ObjetoDeJogo("Key", Sprite("rsc/Sprites/Key.img"), 20, 20), true, 1);
    key->setUseFunction([this, key]() -> bool{
        if (key->getHolder()->colideCom(*door) && !door->isOpened()) {
            door->setOpened(true);
            return true;
        }
        return false;
    });
    items.push_back(key);

    Item* rune1 = new Item(ObjetoDeJogo("Rune", Sprite("rsc/Sprites/Rune.img"), 40, 30), true, 1);
    rune1->setUseFunction([this, rune1]() -> bool{
        if (rune1->getHolder()->colideCom(*door) && !door->isOpened()) {
            door->addRune();
            if (door->checkRunes())
                door->setOpened(true);
            return true;
        }
        return false;
    });
    items.push_back(rune1);

    Item* rune2 = new Item(ObjetoDeJogo("Rune", Sprite("rsc/Sprites/Rune.img"), 40, 40), true, 1);
    rune2->setUseFunction([this, rune2]() -> bool{
        if (rune2->getHolder()->colideCom(*door) && !door->isOpened()) {
            door->addRune();
            if (door->checkRunes())
                door->setOpened(true);
            return true;
        }
        return false;
    });
    items.push_back(rune2);

    Item* rune3 = new Item(ObjetoDeJogo("Rune", Sprite("rsc/Sprites/Rune.img"), 40, 50), true, 1);
    rune3->setUseFunction([this, rune3]() -> bool{
        if (rune3->getHolder()->colideCom(*door) && !door->isOpened()) {
            door->addRune();
            if (door->checkRunes())
                door->setOpened(true);
            return true;
        }
        return false;
    });
    items.push_back(rune3);

    Item* slingshot = new Item(ObjetoDeJogo("Slingshot", Sprite("rsc/Sprites/Slingshot.img"), 20, 50));
    slingshot->setUseFunction([this, slingshot]() -> bool {
        controller->createBullet(slingshot->getHolder(), 10, 20, 1);
        return true;
    });
    items.push_back(slingshot);

    // Barras
    healthBar = new Bars(60, 267, player->getHealth());
    defenseBar = new Bars(68, 267, player->getDefense());
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
                if (posL < screen.getAltura() - player->getSprite()->getAltura() - 19) {
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
                if (posC < screen.getLargura(0) - player->getSprite()->getLarguraMax() - 4) {
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

        if (ch == '1' || ch == '2' || ch == '3' || ch == '4' || ch == '5' || ch == '6') {
            int index = ch - '1';

            if (player->isSelectedItem(index)) {
                player->deselectItem();
                selectionItem->desativarObj();
            } else {
                player->selectItem(index);
                selectionItem->moveTo(56, 7 + index * 38);
                selectionItem->ativarObj();
            }
        }


        update(screen);
        draw(screen);
        system("clear");
        show(screen);

        if (player->getHealth() <= 0) {
            return Fase::GAME_OVER;
        }
    }

    return 0;
}

void FaseUm::update(SpriteBuffer &screen) {
    controller->update(screen);

    for (auto obj = objs.begin(); obj != objs.end(); ++obj) {
        (*obj)->update();
    }

    for (auto item = items.begin(); item != items.end(); ) {
        Item* i = *item;
        if (i->getRemainingUses() == 0) {
            item = items.erase(item);
            i->desativarObj();
            i->getHolder()->removeItem();
            delete i;
        } else {
            ++item;
        }
    }

    healthBar->update(player->getHealth());
    defenseBar->update(player->getDefense());
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
    map->draw(screen);

    for (auto obj : objs) {
        obj->draw(screen, obj->getPosL(), obj->getPosC());
    }

    for (auto item : items) {
        item->draw(screen, item->getPosL(), item->getPosC());
    }

    healthBar->draw(screen);
    defenseBar->draw(screen);
    controller->draw(screen);
    player->draw(screen, player->getPosL(), player->getPosC());
}