#ifndef FASEDOIS_HPP
#define FASEDOIS_HPP

#include "../ASCII_Engine/Fase.hpp"
#include "../Core/Controller.hpp"
#include "../Core/Inventory.hpp"
#include "../Core/Player.hpp"
#include "../Core/Entity.hpp"
#include "../Core/Bullet.hpp"
#include "../Core/Enemy.hpp"
#include "../Core/Altar.hpp"
#include "../Core/Item.hpp"
#include "../Core/Door.hpp"
#include "../Core/Bars.hpp"
#include "../Core/Map.hpp"
#include <string>
#include <vector>
#include <list>

class FaseDois : public Fase {
    private:
        list<ObjetoDeJogo*> colisoes;
        Bars *healthBar, *defenseBar;
        ObjetoDeJogo *selectionItem, *trap1, *trap2, *switch1, *switch2;
        Controller* controller;
        list<Altar*> altars;
        list<Item*> items;
        Player* player;
        Door* portal;
        Map* map;
        
    public:
        FaseDois(string name, const SpriteBase &bkg, Map* map) : Fase(name, bkg), map(map) {}
        FaseDois(string name, const SpriteAnimado &bkg, Map* map) : Fase(name, bkg), map(map) {}

        virtual ~FaseDois() {}

        virtual void init();
        virtual unsigned run(SpriteBuffer &screen);

        virtual void update(SpriteBuffer &screen);

        bool colissionObjs() const;
        Altar* getColissionAltar() const;
        Item* getColissionItem() const;
        virtual void draw(SpriteBase &screen, int x = 0, int y = 0) override;

};

#endif