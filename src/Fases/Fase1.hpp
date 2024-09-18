#ifndef FASEUM_HPP
#define FASEUM_HPP

#include <string>
#include <list>
#include <vector>
#include "../ASCII_Engine/Fase.hpp"
#include "../Core/Player.hpp"
#include "../Core/Entity.hpp"
#include "../Core/Item.hpp"
#include "../Core/Door.hpp"
#include "../Core/Bullet.hpp"
#include "../Core/Inventory.hpp"

using namespace std;

class FaseUm : public Fase {
    private:
        list<Entity*> entities;
        list<Item*> items;
        list<Bullet*> bullets;
        list<ObjetoDeJogo*> colisoes;
        ObjetoDeJogo *selectionItem, *colisionDoor;
        Player* player;
        Door *door;

    public:
        FaseUm(string name, const SpriteBase &bkg) : Fase(name, bkg) {}
        FaseUm(string name, const SpriteAnimado &bkg) : Fase(name, bkg) {}

        virtual ~FaseUm() {}

        virtual void init();
        virtual unsigned run(SpriteBuffer &screen);

        void bulletUpdate(SpriteBuffer &screen);
        void itemsUpdate();
        void entitiesUpdate();

        bool colissionObjs() const;
        Item* getColissionItem() const;
        virtual void draw(SpriteBase &screen, int x = 0, int y = 0) override;
};

#endif // FASEUM_HPP