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
        Player(const int& posL, const int& posC) 
            : Entity(ObjetoDeJogo("Player", Sprite("rsc/Sprites/Player.img"), posL, posC), 200, 10, 250), cooldownDurationMs(250) {
            lastUseTime = std::chrono::steady_clock::now() - std::chrono::milliseconds(cooldownDurationMs); 
        }
        
        virtual ~Player() {}

        void addItem(Item* item) {
            if (item != nullptr) {
                inventory.addItem(item, this);
            }
        }

        void addHealth(int health) {
            this->health += health;
            if (this->health > 200) {
                this->health = 200;
            }
        }

        void removeItem() {
            inventory.removeItem(this->getCenterL(), this->getCenterC());
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