#ifndef ITEM_HPP
#define ITEM_HPP

#include <functional>
#include "../ASCII_Engine/ObjetoDeJogo.hpp"
#include "../ASCII_Engine/Sprite.hpp"

using namespace std;

class Item {
    private:
        ObjetoDeJogo* objeto;
        bool usable;
        int damage;
        function<void()> useFunction;

    public:
        Item(ObjetoDeJogo* obj = nullptr, int damage = 0, bool usable = false, int id = 0) 
            : objeto(obj), damage(damage), usable(usable) {}

        void setObjeto(ObjetoDeJogo* obj) {
            objeto = obj;
        }

        ObjetoDeJogo* getObjeto() const {
            return objeto;
        }

        string getName() const {
            return objeto->getName();
        }

        void setDamage(int dmg) {
            damage = dmg;
        }

        int getDamage() const {
            return damage;
        }

        void setUsable(bool use) {
            usable = use;
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

        void dropItem(const int& posL, const int& posC) {
            moveTo(posL, posC);
        }

        void moveTo(const int& l, const int& c) const {
            if (objeto) {
                objeto->moveTo(l, c);
            }
        }

        void centralizarItem(const int& alturaCaixa, const int& larguraCaixa, const int& posL, const int& posC) const {
            int larguraObjeto = objeto->getSprite()->getLarguraMax();
            int alturaObjeto = objeto->getSprite()->getAltura();

            if (alturaObjeto > alturaCaixa) {
                alturaObjeto = alturaCaixa;
            }

            if (larguraObjeto > larguraCaixa) {
                larguraObjeto = larguraCaixa;
            }

            int novaPosL = posL + (alturaCaixa - alturaObjeto) / 2;
            int novaPosC = posC + (larguraCaixa - larguraObjeto) / 2;

            moveTo(novaPosL, novaPosC);
        }
};

#endif // ITEM_HPP