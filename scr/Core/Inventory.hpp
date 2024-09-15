#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include "Item.hpp"
#include <vector>
#include <string>

class Inventory {
    private:
        std::vector<Item> items;
        Item* itemSelected;
        int maxItems;

    public:
        Inventory(int maxItems) : maxItems(maxItems) {}
        ~Inventory() {}

        void addItem(Item item) {
            if (items.size() < maxItems) {
                items.push_back(item);
                item.coletarItem();
            }
        }
};

#endif // INVENTORY_HPP