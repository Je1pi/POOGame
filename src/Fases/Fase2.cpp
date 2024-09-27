#include "Fase2.hpp"
#include "../Core/CaptureKey.hpp"

void FaseDois::init() {
    // Objetos
    selectionItem = new ObjetoDeJogo("SelectionItem", Sprite("rsc/Sprites/SelectedItem.img"), 30, 5);
    selectionItem->desativarObj();
    objs.push_back(selectionItem);

    trap1 = new ObjetoDeJogo("Trap", TextSprite("^^^^^^^^^^^^", COR::AMARELA), 44, 5);
    objs.push_back(trap1);

    trap2 = new ObjetoDeJogo("Trap", TextSprite("^^^^^^^^^^^^^", COR::VERMELHA), 12, 5);
    objs.push_back(trap2);

    switch1 = new ObjetoDeJogo("Interruptor", SpriteAnimado("rsc/SpritesAnimados/Switch.anm", 1, COR::AMARELA), 48, 229);
    objs.push_back(switch1);

    switch2 = new ObjetoDeJogo("Interruptor", SpriteAnimado("rsc/SpritesAnimados/SwitchInverted.anm", 1, COR::VERMELHA), 6, 100);
    objs.push_back(switch2);

    portal = new Door(ObjetoDeJogo("Portal", SpriteAnimado("rsc/SpritesAnimados/Portal.anm"), 25, 160));
    objs.push_back(portal);

    // Item
    Item* slingshot = new Item(ObjetoDeJogo("Slingshot", Sprite("rsc/Sprites/Slingshot.img"), 14, 188));
    slingshot->setUseFunction([this, slingshot]() -> bool {
        controller->createBullet(slingshot->getHolder(), 10, 20, 0);
        return true;
    });
    items.push_back(slingshot);

    Item* potion1 = new Item(ObjetoDeJogo("Potion", Sprite("rsc/Sprites/Potion.img"), 14, 137), true, 1);
    potion1->setUseFunction([this]() -> bool{
        player->addHealth(20);
        return true;
    });
    items.push_back(potion1);

    Item* potion2 = new Item(ObjetoDeJogo("Potion", Sprite("rsc/Sprites/Potion.img"), 36, 137), true, 1);
    potion2->setUseFunction([this]() -> bool{
        player->addHealth(20);
        return true;
    });
    items.push_back(potion2);

    Item* shield = new Item(ObjetoDeJogo("Shield", Sprite("rsc/Sprites/Shield.img"), 37, 185), true, 1);
    shield->setUseFunction([this]() -> bool{
        player->addDefense(60);
        return true;
    });
    items.push_back(shield);

    // Runas e Altares
    Item* rune1 = new Item(ObjetoDeJogo("Rune", Sprite("rsc/Sprites/Rune.img", COR::AMARELA), 14, 86), true, 1);
    Altar* altar1 = new Altar(26, 320, rune1, COR::AMARELA);
    rune1->setUseFunction([this]() -> bool {
        if (player->colideCom(*switch1)) {
            trap1->desativarObj();
            switch1->update();

            return true;
        }
        return false;
    });
    altars.push_back(altar1);
    items.push_back(rune1);

    Item* rune2 = new Item(ObjetoDeJogo("Rune", Sprite("rsc/Sprites/Rune.img", COR::VERMELHA), 4, 40), true, 1);
    Altar* altar2 = new Altar(36, 320, rune2, COR::VERMELHA);
    rune2->setUseFunction([this]() -> bool {
        if (player->colideCom(*switch2)) {
            trap2->desativarObj();
            switch2->update();

            return true;
        }
        return false;
    });
    altars.push_back(altar2);
    items.push_back(rune2);

    Item* rune3 = new Item(ObjetoDeJogo("Rune", Sprite("rsc/Sprites/Rune.img", COR::VERDE), 4, 21), true, 1);
    Altar* altar3 = new Altar(46, 320, rune3, COR::VERDE);
    altars.push_back(altar3);
    items.push_back(rune3);

    // Entidade
    player = new Player(25, 162);
    controller = new Controller(player, map, true);

    Entity* enemy1 = new Enemy(15, 111);
    controller->insertEntity(enemy1);

    Entity* enemy2 = new Enemy(35, 84);
    controller->insertEntity(enemy2);

    Entity* enemy3 = new Enemy(24, 62);
    controller->insertEntity(enemy3);

    Entity* enemy4 = new Enemy(24, 7);
    controller->insertEntity(enemy4);

    Entity* enemy5 = new Enemy(34, 214);
    controller->insertEntity(enemy5);

    Entity* enemy6 = new Enemy(26, 303);
    controller->insertEntity(enemy6);

    Entity* enemy7 = new Enemy(36, 303);
    controller->insertEntity(enemy7);

    Entity* enemy8 = new Enemy(46, 303);
    controller->insertEntity(enemy8);

    Entity* enemy9 = new Enemy(4, 85);
    controller->insertEntity(enemy9);

    Entity* enemy10 = new Enemy(46, 193);
    controller->insertEntity(enemy10);

    Entity* enemy11 = new Enemy(4, 127);
    controller->insertEntity(enemy11);

    Entity* enemy12 = new Enemy(4, 309);
    controller->insertEntity(enemy12);

    // Barras
    healthBar = new Bars(60, 267, player->getHealth(), COR::VERMELHA);
    defenseBar = new Bars(68, 267, player->getDefense(), COR::CIANO);
}

unsigned FaseDois::run(SpriteBuffer &screen) {
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

        if (map->colission(player)) {
            player->moveTo(posL, posC);
        }

        update(screen);
        draw(screen);
        system("clear");
        show(screen);

        if (player->colideCom(*portal) && portal->isOpened()) {
            return Fase::LEVEL_COMPLETE;
        }

        if (player->getHealth() <= 0) {
            return Fase::GAME_OVER;
        }

        if (player->colideCom(*trap1) || player->colideCom(*trap2)) {
            player->addHealth(-100);
        }
    }
    return 0;
}

void FaseDois::update(SpriteBuffer &screen) {
    controller->update(screen);

    bool allAltarsActive = true;

    for (auto altar : altars) {
        if (!altar->isActive()) {
            allAltarsActive = false;
            break;
        }
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

    if (allAltarsActive && !portal->isOpened()) {
        portal->update();
        portal->setOpened(true);
    }

    healthBar->update(player->getHealth());
    defenseBar->update(player->getDefense());
}

void FaseDois::draw(SpriteBase &screen, int x, int y) {
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

    healthBar->draw(screen);
    defenseBar->draw(screen);
    controller->draw(screen);
    player->draw(screen, player->getPosL(), player->getPosC());
}

bool FaseDois::colissionObjs() const {
    for (auto obj = colisoes.begin(); obj != colisoes.end(); ++obj) {
        if (player->colideCom(**obj)) {
            return true;
        }
    }
    return false;
}

Altar* FaseDois::getColissionAltar() const {
    for (auto altar = altars.begin(); altar != altars.end(); ++altar)
        if (player->colideCom(**altar)) {
            return *altar;
        }
    return nullptr;
}

Item* FaseDois::getColissionItem() const {
    for (auto item = items.begin(); item != items.end(); ++item)
        if (player->colideCom(**item)) {
            return *item;
        }
    return nullptr;
}