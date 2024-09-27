#ifndef ALTAR_HPP
#define ALTAR_HPP

#include "Item.hpp"
#include "Player.hpp"
#include "../ASCII_Engine/Cores.hpp"

class Altar : public ObjetoDeJogo{
    private:
        Item* item, *ref;
        bool active = false;
        COR::Cor cor;

    public:
        Altar(const int& posL, const int& posC, Item* ref = nullptr, COR::Cor color = COR::PADRAO) : ObjetoDeJogo("Altar", Sprite("rsc/Sprites/Altar.img", color), posL, posC), item(nullptr), ref(ref), cor(color) {}
        virtual ~Altar() {}

        void setItem(Item* item) {
            this->item = item;
        }

        Item* getItem() const {
            return item;
        }

        void removeItem() {
            item = nullptr;
        }
        
        bool isActive() const {
            return active;
        }

        void transferItem(Player* player) {
            if (item != nullptr && this->active) {
                item->ativarObj();
                player->addItem(item);
                removeItem();
                this->setCor(cor);
                this->active = false;
            }
        }

        void receiveItem(Player* player) {
            bool receive = false;

            if (player->getSelectedItem() != nullptr && this->item == nullptr) {
                Item *temp = player->getSelectedItem();
                
                if (ref != nullptr) {
                    if (temp == ref) receive = true;

                } else {
                    if (item->getName() == "Rune") receive = true;
                }

                if (receive) {
                    player->removeItem();
                    setItem(temp);
                    item->moveTo(this->getPosL(), this->getPosC());
                    item->desativarObj();
                    this->setCor(COR::CIANO);
                    this->active = true;
                }
            }
        }
};

#endif