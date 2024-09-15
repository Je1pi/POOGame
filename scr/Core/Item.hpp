#ifndef ITEM_HPP
#define ITEM_HPP

#include "../ASCII_Engine/ObjetoDeJogo.hpp"

using namespace std;

class Item {
    private:
        ObjetoDeJogo* objeto;
        int damage;

    public:
        Item(ObjetoDeJogo* obj = nullptr, int damage = 0) : objeto(obj), damage(damage) {}

        void setObjeto(ObjetoDeJogo* obj) {
            objeto = obj;
        }

        ObjetoDeJogo* getObjeto() const {
            return objeto;
        }

        void setDamage(int dmg) {
            damage = dmg;
        }

        int getDamage() const {
            return damage;
        }

        void coletarItem() {
            if (objeto) {
                objeto->desativarObj();
            }
        }
};

#endif