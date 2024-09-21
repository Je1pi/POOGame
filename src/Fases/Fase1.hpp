#ifndef FASEUM_HPP
#define FASEUM_HPP

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

#include <fstream>

using namespace std;

class FaseUm : public Fase {
    private:
        ObjetoDeJogo *selectionItem, *checkKeyDoor = nullptr;
        Bars *healthBar, *defenseBar;
        list<ObjetoDeJogo*> colisoes;
        Controller* controller;
        list<Altar*> altars;
        list<Item*> items;
        Player* player;
        Door *door1, *door2, *portal;
        Map* map;

    public:
        FaseUm(string name, const SpriteBase &bkg, Map* map) : Fase(name, bkg), map(map) {}
        FaseUm(string name, const SpriteAnimado &bkg, Map* map) : Fase(name, bkg), map(map) {}

        virtual ~FaseUm() {}

        void debug(const string& msg) {
            ofstream debugFile("debug.txt", ios::app);
            if (debugFile.is_open()) {
                debugFile << msg << endl;
                debugFile.close();
            } else {
                cerr << "Não foi possível abrir o arquivo debug.txt para escrita." << endl;
            }
        }

        virtual void init();
        virtual unsigned run(SpriteBuffer &screen);

        virtual void update(SpriteBuffer &screen);

        bool colissionObjs() const;
        Altar* getColissionAltar() const;
        Item* getColissionItem() const;
        virtual void draw(SpriteBase &screen, int x = 0, int y = 0) override;
};

#endif // FASEUM_HPP