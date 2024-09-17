#ifndef DOOR_HPP
#define DOOR_HPP

#include "../ASCII_Engine/ObjetoDeJogo.hpp"

using namespace std;

class Door : public ObjetoDeJogo {
    private:
        bool opened = false;
    
    public:
        Door(string name, const SpriteBase &sprite, int posL, int posC) : ObjetoDeJogo(name, sprite, posL, posC) {}
        Door(string name, const SpriteAnimado &sprite, int posL, int posC) : ObjetoDeJogo(name, sprite, posL, posC) {}

        virtual ~Door() {}

        bool isOpened() const { return opened; }
};

#endif // DOOR_HPP