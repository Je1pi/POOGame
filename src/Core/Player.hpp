#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Entity.hpp"
#include "Inventory.hpp"
#include <chrono>

using namespace std;

class Player : public Entity {
    private:
        Inventory inventory;
        std::chrono::steady_clock::time_point lastUseTime;
        int cooldownDurationMs;

    public:
        Player(const ObjetoDeJogo &obj, int life = 100, int damage = 10, int defense = 5) 
            : Entity(obj, life, damage, defense), cooldownDurationMs(1000) {
            lastUseTime = std::chrono::steady_clock::now() - std::chrono::milliseconds(cooldownDurationMs); 
        }
        
        virtual ~Player() {}

        void addItem(Item* item) {
            if (item != nullptr) {
                inventory.addItem(item, this);
            }
        }

        void removeItem() {
            inventory.removeItem(this->getPosL(), this->getPosC());
        }

        void selectItem(int index) {
            inventory.selectItem(index);
        }

        void deselectItem() {
            inventory.deselectItem();
        }

        bool isSelectedItem(int index) const {
            return index == inventory.getSelectedItemIndex();
        }

        Item* getSelectedItem() const {
            return inventory.getItem();
        }

        void useItem() {
            auto now = std::chrono::steady_clock::now();
            auto timeSinceLastUse = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUseTime).count();

            if (timeSinceLastUse >= cooldownDurationMs) {
                Item* item = getSelectedItem();
                if (item != nullptr) {
                    item->use();
                    lastUseTime = now;
                }
            }
        }
};

#endif // PLAYER_HPP