#include "Fase1.hpp"
#include <iostream>
#include "../Core/captureKey.hpp"

void FaseUm::init() {
    items.push_back(new Item(new ObjetoDeJogo("Key1", Sprite("rsc/Sprites/Key.img"), 20, 20), 0, true));
    items.push_back(new Item(new ObjetoDeJogo("Key2", Sprite("rsc/Sprites/Key.img"), 20, 30), 0, true));

    pointer = new ObjetoDeJogo("Pointer", TextSprite("*"), 31, 6);
    pointer->desativarObj();
    objs.push_back(pointer);

    player = new Player(ObjetoDeJogo("Player", Sprite("rsc/Sprites/Player.img"), 10, 10));
    objs.push_back(player);
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

        if (ch == 'w' && posL > 2) {
            player->moveUp(1);
        } else if (ch == 's' && posL < screen.getAltura() - 11) {
            player->moveDown(1);
        } else if (ch == 'a' && posC > 3) {
            player->moveLeft(1);
        } else if (ch == 'd' && posC < screen.getLargura(0) - 5) {
            player->moveRight(1);
        }

        if (ch == 'j') {
            Item* item = getColissionItem();
            if (item != nullptr) {
                player->addItem(item);
            }
        }

        if (ch == '1' || ch == '2' || ch == '3' || ch == '4' || ch == '5') {
            int index = ch - '1';

            if (player->selectItem(index)) {
                pointer->moveTo(31, 6 + index * 14);
                pointer->ativarObj();
            } else {
                pointer->desativarObj();
            }
        }

        if (ch == 'k') {
            player->removeItem();
            pointer->desativarObj();
        }

        update();
        draw(screen);
        system("clear");
        show(screen);
    }

    return 0;
}

bool FaseUm::colissionItems() const {
    for (const auto& item : items) {
        ObjetoDeJogo* obj = item->getObjeto();
        if (obj && player->colideCom(*obj)) {
            return true;
        }
    }
    return false;
}

Item* FaseUm::getColissionItem() const {
    for (const auto& item : items) {
        ObjetoDeJogo* obj = item->getObjeto();
        if (obj && player->colideCom(*obj)) {
            return item;
        }
    }
    return nullptr;
}

void FaseUm::draw(SpriteBase &screen, int x, int y) {
    background->draw(screen, 0, 0);

    for (auto item : items) {
        ObjetoDeJogo* obj = item->getObjeto();
        if (obj) {
            obj->draw(screen, obj->getPosL(), obj->getPosC());
        }
    }

    for (auto obj : objs) {
        obj->draw(screen, obj->getPosL(), obj->getPosC());
    }
}