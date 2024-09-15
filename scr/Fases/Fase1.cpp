#include "Fase1.hpp"

#include <iostream>
#include "../Core/captureKey.hpp"

void FaseUm::init() {
    key = new ObjetoDeJogo("Key", Sprite("rsc/Sprites/Key.img"), 20, 20);
    objs.push_back(key);

    door = new Door("Door", SpriteAnimado("rsc/SpritesAnimados/Door.anm"), 30, 30);
    objs.push_back(door);
    
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
            } else if (ch == 's' && posL < screen.getAltura() - 10) {
                player->moveDown(1);
            } else if (ch == 'a' && posC > 3) {
                player->moveLeft(1);
            } else if (ch == 'd' && posC < screen.getLargura(0) - 5) {
                player->moveRight(1);
            }

            if (player->colideCom(*key) && ch == 'j') {
                player->addItem(Item(key, 0));
            }

			update();
		    draw(screen);
		    system("clear");
		    show(screen);
		}
    
    return 0;
}