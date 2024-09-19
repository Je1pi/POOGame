#ifndef DOOR_HPP
#define DOOR_HPP

#include "../ASCII_Engine/ObjetoDeJogo.hpp"
#include "Item.hpp"

using namespace std;

class Door : public ObjetoDeJogo {
    private:
        bool opened = false;
        int runes;
    
    public:
        Door (ObjetoDeJogo obj) : ObjetoDeJogo(obj), runes(0) {}

        virtual ~Door () {}

        bool isOpened () const { return opened; }

        void setOpened (bool value) { opened = value; }

        void addRune () {
            if (runes < 3) {
                runes++;
            }
        }

        bool checkRunes () {
            return runes == 3;
            return true;
        }
};

#endif // DOOR_HPP