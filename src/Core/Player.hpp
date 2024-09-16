#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "../ASCII_Engine/ObjetoDeJogo.hpp"
#include "Rune.hpp"
#include "Inventory.hpp"

using namespace std;

class Player : public ObjetoDeJogo {
    private:
        int life, defense;
        Inventory inventory;
        int selectedItemIndex;

    public:
        Player(const ObjetoDeJogo &obj) 
            : ObjetoDeJogo(obj), life(100), defense(10), selectedItemIndex(-1) {}

        virtual ~Player() {}

        void addItem(Item* item) {
            if (item != nullptr) {
                inventory.addItem(item);
            }
        }

        void removeItem() {
            if (selectedItemIndex >= 0 && selectedItemIndex < inventory.getSize()) {
                inventory.removeItem(selectedItemIndex);
                selectedItemIndex = -1;
            }
        }

        bool selectItem(int index) {
            if (index >= 0 && index < inventory.getSize() && !inventory.isSlotEmpty(index)) {
                selectedItemIndex = index;
                return true;
            }
            return false;
        }

        Item* getSelectedItem() const {
            return inventory.getItem(selectedItemIndex);
        }
};

#endif // PLAYER_HPP