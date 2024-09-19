#ifndef FASEUM_HPP
#define FASEUM_HPP

#include "../ASCII_Engine/Fase.hpp"
#include "../Core/Controller.hpp"
#include "../Core/Inventory.hpp"
#include "../Core/Player.hpp"
#include "../Core/Entity.hpp"
#include "../Core/Bullet.hpp"
#include "../Core/Enemy.hpp"
#include "../Core/Item.hpp"
#include "../Core/Door.hpp"
#include "../Core/Bars.hpp"
#include "../Core/Map.hpp"
#include <string>
#include <vector>
#include <list>

using namespace std;

class FaseUm : public Fase {
    private:
        ObjetoDeJogo *selectionItem, *colisionDoor;
        Bars *healthBar, *defenseBar;
        list<ObjetoDeJogo*> colisoes;
        Controller* controller;
        list<Item*> items;
        Player* player;
        Door *door;
        Map* map;

    public:
        FaseUm(string name, const SpriteBase &bkg, Map* map) : Fase(name, bkg), map(map) {}
        FaseUm(string name, const SpriteAnimado &bkg, Map* map) : Fase(name, bkg), map(map) {}

        virtual ~FaseUm() {}

        virtual void init();
        virtual unsigned run(SpriteBuffer &screen);

        virtual void update(SpriteBuffer &screen);

        bool colissionObjs() const;
        Item* getColissionItem() const;
        virtual void draw(SpriteBase &screen, int x = 0, int y = 0) override;
};

#endif // FASEUM_HPP