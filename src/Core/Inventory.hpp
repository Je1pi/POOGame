#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include "Item.hpp"
#include <vector>

class Inventory {
    private:
        static const int MAX_ITEMS = 5;
        Item* items[MAX_ITEMS];

    public:
        Inventory() {
            for (int i = 0; i < MAX_ITEMS; ++i) {
                items[i] = nullptr;
            }
        }

        void addItem(Item* item) {
            for (int i = 0; i < MAX_ITEMS; ++i) {
                if (items[i] == nullptr) {
                    items[i] = item;
                    return;
                }
            }
        }


        void removeItem(int index) {
            if (index >= 0 && index < MAX_ITEMS && items[index] != nullptr) {
                delete items[index];
                items[index] = nullptr;
            }
        }

        Item* getItem(int index) const {
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
};

#endif // INVENTORY_HPP