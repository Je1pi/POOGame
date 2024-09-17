#ifndef ITEM_HPP
#define ITEM_HPP

#include <functional>
#include "../ASCII_Engine/ObjetoDeJogo.hpp"
#include "Entity.hpp"
#include "../ASCII_Engine/Sprite.hpp"

using namespace std;

class Item : public ObjetoDeJogo {
    private:
        bool usable;
        int damage;
        Entity *holder;
        function<void()> useFunction;

    public:
        Item(const ObjetoDeJogo& obj, const int& dmg = 0, const bool& usable = false) 
            : ObjetoDeJogo(obj), damage(dmg), usable(usable), holder(nullptr) {}

        void setDamage(int dmg) {
            damage = dmg;
        }

        int getDamage() const {
            return damage;
        }

        void setUsable(bool use) {
            usable = use;
        }

        void setHolder(Entity* entity) {
            holder = entity;
        }

        Entity* getHolder() const {
            return holder;
        }

        void removeHolder() {
            holder = nullptr;
        }

        bool isUsable() const {
            return usable;
        }

        void setUseFunction(function<void()> func) {
            useFunction = func;
        }

        void use() {
            if (usable && useFunction) {
                useFunction();
            }
        }

        void centralizarItem(const int& alturaCaixa, const int& larguraCaixa, const int& posL, const int& posC) {
            int larguraObjeto = this->getSprite()->getLarguraMax();
            int alturaObjeto = this->getSprite()->getAltura();

            if (alturaObjeto > alturaCaixa) {
                alturaObjeto = alturaCaixa;
            }

            if (larguraObjeto > larguraCaixa) {
                larguraObjeto = larguraCaixa;
            }

            int novaPosL = posL + (alturaCaixa - alturaObjeto) / 2;
            int novaPosC = posC + (larguraCaixa - larguraObjeto) / 2;

            this->moveTo(novaPosL, novaPosC);
        }
};

#endif // ITEM_HPP