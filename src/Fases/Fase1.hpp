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
        list<Item*> items;
        ObjetoDeJogo* pointer;
        Door *door;

    public:
        FaseUm(string name, const SpriteBase &bkg) : Fase(name, bkg) {}
        FaseUm(string name, const SpriteAnimado &bkg) : Fase(name, bkg) {}

        virtual ~FaseUm() {}

        virtual void init();
        virtual unsigned run(SpriteBuffer &screen);

        bool colissionItems() const;
        Item* getColissionItem() const;
        virtual void draw(SpriteBase &screen, int x = 0, int y = 0) override;
};

#endif // FASEUM_HPP