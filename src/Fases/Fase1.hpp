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
#include "../Core/Enemy.hpp"

using namespace std;

class FaseUm : public Fase {
    private:
        list<Entity*> entities;
        list<Item*> items;
        list<Bullet*> bullets;
        list<ObjetoDeJogo*> colisoes;
        SpriteBase* map;
        ObjetoDeJogo *selectionItem, *colisionDoor;
        Player* player;
        Door *door;

    public:
        FaseUm(string name, const SpriteBase &bkg, const SpriteBase &map) : Fase(name, bkg), map(map.copia()) {}
        FaseUm(string name, const SpriteAnimado &bkg, const SpriteAnimado &map) : Fase(name, bkg), map(map.copia()) {}

        virtual ~FaseUm() {}

        virtual void init();
        virtual unsigned run(SpriteBuffer &screen);

        void addBullet(Bullet* bullet) { bullets.push_back(bullet); }

        void bulletUpdate(SpriteBuffer &screen);
        void itemsUpdate();
        void entitiesUpdate();
        void statsUpdate();

        bool colissionObjs() const;
        Item* getColissionItem() const;
        virtual void draw(SpriteBase &screen, int x = 0, int y = 0) override;
};

#endif // FASEUM_HPP