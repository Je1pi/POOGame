#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include "Item.hpp"
#include "Player.hpp"
#include "../ASCII_Engine/Sprite.hpp"
#include "../ASCII_Engine/ObjetoDeJogo.hpp"

class Inventory {
    private:
        int selectedItemIndex;
        static const int MAX_ITEMS = 6;
        Item* items[MAX_ITEMS];

    public:
        Inventory() : selectedItemIndex(-1) {
            for (int i = 0; i < MAX_ITEMS; ++i) {
                items[i] = nullptr;
            }
        }

        void addItem(Item* item, Player* entity) {
            for (int i = 0; i < MAX_ITEMS; ++i) {
                if (items[i] == nullptr) {
                    items[i] = item;

                    item->setHolder(entity);
                    item->centralizarItem(17, 31, 56, 7 + i * 38);

                    return;
                }
            }
        }
        
        void removeItem(int posL, int posC) {
            int index = selectedItemIndex;
            if (index >= 0 && index < MAX_ITEMS && items[index] != nullptr) {
                items[index]->moveTo(posL, posC);
                items[index]->removeHolder();
                items[index] = nullptr;
            }
        }

        Item* getItem() const {
            int index = selectedItemIndex;
            if (index >= 0 && index < MAX_ITEMS) {
                return items[index];
            }
            return nullptr;
        }

        bool isSlotEmpty(int index) const {
            if (index >= 0 && index < MAX_ITEMS) {
                return items[index] == nullptr;
            }
            return false;
        }

        int getSize() const {
            return MAX_ITEMS;
        }

        int getSelectedItemIndex() const {
            return selectedItemIndex;
        }

        void selectItem(int index) {
            selectedItemIndex = index;
        }

        void deselectItem() {
            selectedItemIndex = -1;
        }
};

#endif // INVENTORY_HPP