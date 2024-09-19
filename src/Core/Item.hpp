#ifndef ITEM_HPP
#define ITEM_HPP

#include <functional>
#include "../ASCII_Engine/ObjetoDeJogo.hpp"

using namespace std;

class Player;

class Item : public ObjetoDeJogo {
    private:
        Player* holder;
        bool usable;
        int qntUses;
        function<bool()> useFunction;

    public:
        Item(const ObjetoDeJogo& obj, const bool& usable = true, const int& qntUses = -1) 
            : ObjetoDeJogo(obj), usable(usable), qntUses(qntUses), holder(nullptr) {}

        virtual ~Item() {}

        void setHolder(Player* player) {
            holder = player;
        }

        Player* getHolder() const {
            return holder;
        }

        void removeHolder() {
            holder = nullptr;
        }

        bool isUsable() const {
            return usable;
        }

        void setUseFunction(function<bool()> func) {
            useFunction = func;
        }

        void use() {
            if (usable && useFunction) {
                if (qntUses == -1 || qntUses > 0) {
                    bool success = useFunction();
                    if (success) {
                        if (qntUses > 0) {
                            qntUses--;
                        }
                    }
                }
            }
        }

        bool hasInfiniteUses() const {
            return qntUses == -1;
        }

        int getRemainingUses() const {
            return qntUses;
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

        void safeUse() {
            if (holder != nullptr) {
                use();
            }
        }

        void deleteHolder() {
            holder = nullptr;
        }
};

#endif // ITEM_HPP