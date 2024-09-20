#ifndef ALTAR_HPP
#define ALTAR_HPP

#include "Item.hpp"
#include "Player.hpp"

class Altar : public ObjetoDeJogo{
    private:
        Item* item;
        bool active;
    
    public:
        Altar(const int& posL, const int& posC) : ObjetoDeJogo("Altar", Sprite("rsc/Sprites/Altar.img"), posL, posC), item(nullptr), active(true) {}
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
        
        void setActive(bool active) {
            this->active = active;
        }

        void transferItem(Player* player) {
            if (item != nullptr && this->active) {
                item->ativarObj();
                player->addItem(item);
                removeItem();
                this->setCor(COR::PADRAO);
            }
        }

        void receiveItem(Player* player) {
            if (player->getSelectedItem() != nullptr && this->item == nullptr) {
                setItem(player->getSelectedItem());
                player->removeItem();
                if (item->getName() == "Rune") {
                    this->setCor(COR::VERDE);
                } else {
                    this->setCor(COR::VERMELHA);
                }
                item->desativarObj();
            }
        }
};

#endif