#include "Fase1.hpp"
#include <iostream>
#include "../Core/CaptureKey.hpp"

void FaseUm::init() {
    // Objetos de Jogo
    selectionItem = new ObjetoDeJogo("SelectionItem", Sprite("rsc/Sprites/SelectedItem.img"), 30, 5);
    selectionItem->desativarObj();
    objs.push_back(selectionItem);

    checkKeyDoor = new ObjetoDeJogo("colissionDoor", SpriteBuffer(5, 8), 28, 157);
    objs.push_back(checkKeyDoor);
    
    door1 = new Door(ObjetoDeJogo("Door", SpriteAnimado("rsc/SpritesAnimados/Door.anm"), 27, 159));
    objs.push_back(door1);

    door2 = new Door(ObjetoDeJogo("Door", SpriteAnimado("rsc/SpritesAnimados/Door.anm"), 3, 298));
    objs.push_back(door2);

    portal = new Door(ObjetoDeJogo("Portal", SpriteAnimado("rsc/SpritesAnimados/Portal.anm"), 42, 309));

    // Entidades
    player = new Player(44, 9);
    controller = new Controller(player, map);
    
    Enemy* enemy1 = new Enemy(18, 312);
    controller->insertEntity(enemy1);

    Enemy* enemy2 = new Enemy(24, 55);
    controller->insertEntity(enemy2);

    Enemy* enemy3 = new Enemy(45, 100);
    controller->insertEntity(enemy3);

    Enemy* enemy4 = new Enemy(10, 157);
    controller->insertEntity(enemy4);

    Enemy* enemy5 = new Enemy(43, 174);
    controller->insertEntity(enemy5);

    Enemy* enemy6 = new Enemy(26, 235);
    controller->insertEntity(enemy6);

    Enemy* enemy7 = new Enemy(9, 201);
    controller->insertEntity(enemy7);

    Enemy* enemy8 = new Enemy(44, 242);
    controller->insertEntity(enemy8);

    // Items
    Item* key = new Item(ObjetoDeJogo("Key", Sprite("rsc/Sprites/Key.img"), 21, 77), true, 1);
    key->setUseFunction([this, key]() -> bool{
        if (key->getHolder()->colideCom(*checkKeyDoor) && !door1->isOpened()) {
            door1->setOpened(true);
            door1->update();
            return true;
        }
        return false;
    });
    items.push_back(key);

    Item* potion = new Item(ObjetoDeJogo("Potion", Sprite("rsc/Sprites/Potion.img"), 5, 84), true, 1);
    potion->setUseFunction([this]() -> bool{
        player->addHealth(50);
        return true;
    });
    items.push_back(potion);

    Item* shield = new Item(ObjetoDeJogo("Shield", Sprite("rsc/Sprites/Shield.img"), 34, 252), true, 1);
    shield->setUseFunction([this]() -> bool{
        player->addDefense(60);
        return true;
    });
    items.push_back(shield);

    // Runes and Altars
    Item* rune1 = new Item(ObjetoDeJogo("Rune", Sprite("rsc/Sprites/Rune.img", COR::VERMELHA), 46, 112), true, 1);
    Altar* altar1 = new Altar(45, 139, rune1, COR::VERMELHA);
    altars.push_back(altar1);
    items.push_back(rune1);

    Item* rune2 = new Item(ObjetoDeJogo("Rune", Sprite("rsc/Sprites/Rune.img", COR::VERDE), 6, 185), true, 1);
    Altar* altar2 = new Altar(4, 126, rune2, COR::VERDE);
    altars.push_back(altar2);
    items.push_back(rune2);

    Item* rune3 = new Item(ObjetoDeJogo("Rune", Sprite("rsc/Sprites/Rune.img", COR::AMARELA), 44, 209), true, 1);
    Altar* altar3 = new Altar(4, 8, rune3, COR::AMARELA);
    altars.push_back(altar3);
    items.push_back(rune3);

    // Weapon
    Item* slingshot = new Item(ObjetoDeJogo("Slingshot", Sprite("rsc/Sprites/Slingshot.img"), 46, 34));
    slingshot->setUseFunction([this, slingshot]() -> bool {
        controller->createBullet(slingshot->getHolder(), 10, 20, 0);
        return true;
    });
    items.push_back(slingshot);

    // Barras
    healthBar = new Bars(60, 267, player->getHealth(), COR::VERMELHA);
    defenseBar = new Bars(68, 267, player->getDefense(), COR::CIANO);
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
        Altar* altar = nullptr;

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
                altar = getColissionAltar();
                if (item != nullptr) {
                    player->addItem(item);
                } else {
                    if (altar != nullptr) {
                        altar->transferItem(player);
                    }
                }
                break;

            case 'k':
                player->removeItem();
                break;

            case 'l':
                altar = getColissionAltar();
                if (altar != nullptr) {
                    altar->receiveItem(player);
                } else {
                    player->useItem();
                }
                break;
            
            case 'o':
                return Fase::LEVEL_COMPLETE;
                break;
        }

        if (colissionObjs() || map->colission(player)) {
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

        if (player->colideCom(*portal)) {
            return Fase::LEVEL_COMPLETE;
        }
    }

    return 0;
}

void FaseUm::update(SpriteBuffer &screen) {
    controller->update(screen);

    for (auto obj = objs.begin(); obj != objs.end(); ++obj)
        if ((*obj) != door1 && (*obj) != door2)
            (*obj)->update();

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

    bool allAltarsActive = true;

    for (auto altar : altars) {
        if (!altar->isActive()) {
            allAltarsActive = false;
            break;
        }
    }

    if (allAltarsActive && !door2->isOpened()) {
        door2->setOpened(true);
        door2->update();
        portal->update();
    }
}

bool FaseUm::colissionObjs() const {
    for (auto obj = colisoes.begin(); obj != colisoes.end(); ++obj) {
        if (player->colideCom(**obj)) {
            return true;
        }
    }

    if (player->colideCom(*door1) && !door1->isOpened()) {
        return true;
    }

    if(player->colideCom(*door2) && !door2->isOpened()) {
        return true;
    }

    return false;
}

Altar* FaseUm::getColissionAltar() const {
    for (auto altar = altars.begin(); altar != altars.end(); ++altar)
        if (player->colideCom(**altar)) {
            return *altar;
        }
    return nullptr;
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

    for (auto altar : altars) {
        altar->draw(screen, altar->getPosL(), altar->getPosC());
    }

    portal->draw(screen, portal->getPosL(), portal->getPosC());

    healthBar->draw(screen);
    defenseBar->draw(screen);
    controller->draw(screen);
    player->draw(screen, player->getPosL(), player->getPosC());
}