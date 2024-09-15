#ifndef FASEUM_HPP
#define FASEUM_HPP

#include <string>
#include <list>
#include <vector>
#include "../ASCII_Engine/Fase.hpp"

#include "../Core/Player.hpp"
#include "../Core/Item.hpp"
#include "../Core/Door.hpp"
#include "../Core/Inventory.hpp"

using namespace std;

class FaseUm : public Fase {
    private:
        Player *player;
        list<ObjetoDeJogo*> colisoes;
        ObjetoDeJogo *key;
        Door *door;

    public:
        FaseUm(string name, const SpriteBase &bkg) : Fase(name, bkg) {}
        FaseUm(string name, const SpriteAnimado &bkg) : Fase(name, bkg) {}

        virtual ~FaseUm() {}

        virtual void init();
        virtual unsigned run(SpriteBuffer &screen);
};

#endif // FASEUM_HPP