#include "Fase2.hpp"
#include "../Core/CaptureKey.hpp"

void FaseDois::init() {
    selectionItem = new ObjetoDeJogo("SelectionItem", Sprite("rsc/Sprites/SelectedItem.img"), 30, 5);
    selectionItem->desativarObj();
    objs.push_back(selectionItem);

    // Reiniciando o jogador para nova Fase
    player = new Player(25, 162);
    player->deselectItem();
    //player->addHealth(200);
    //player->addDefense(250);
    controller = new Controller(player, map);

    // Barras
    healthBar = new Bars(60, 267, player->getHealth());
    defenseBar = new Bars(68, 267, player->getDefense());
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
            
            case 'p':
                debug("Player Position: " + to_string(player->getPosL()) + ", " + to_string(player->getPosC()));
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
    }
    return 0;
}

void FaseDois::update(SpriteBuffer &screen) {
    controller->update(screen);
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