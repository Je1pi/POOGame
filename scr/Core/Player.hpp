#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "../ASCII_Engine/ObjetoDeJogo.hpp"
#include <vector>
#include "Rune.hpp"
#include "Inventory.hpp"

using namespace std;

class Player : public ObjetoDeJogo{
    private:
        int life;
        int velocity;
        Inventory inventory;
        vector<Rune> runes;
        int maxRunes = 3;

    public:
        Player(const ObjetoDeJogo &obj) : ObjetoDeJogo(obj), life(100), velocity(10), inventory(5) {}
        virtual ~Player() {}

        void addItem(Item item) {
            inventory.addItem(item);
        }
};

#endif // PLAYER_HPP