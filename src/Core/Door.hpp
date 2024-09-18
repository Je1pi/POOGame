#ifndef DOOR_HPP
#define DOOR_HPP

#include "../ASCII_Engine/ObjetoDeJogo.hpp"
#include "Item.hpp"

using namespace std;

class Door : public ObjetoDeJogo {
    private:
        bool opened = false;
        Item* runes[3];
    
    public:
        Door(ObjetoDeJogo obj) : ObjetoDeJogo(obj) {
            for (int i = 0; i < 3; i++) {
                runes[i] = nullptr;
            }
        }

        virtual ~Door() {}

        bool isOpened() const { return opened; }

        void setOpened(bool value) { opened = value; }

        
};

#endif // DOOR_HPP