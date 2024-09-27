#ifndef GAMERESULT_HPP
#define GAMERESULT_HPP

#include "../ASCII_Engine/Fase.hpp"

using namespace std;

class GameResult : public Fase {
    public:
        GameResult(string name, const SpriteBase &bkg) : Fase(name, bkg) {}
        virtual ~GameResult() {}

        virtual void init() { /* do nothing */ }
        virtual unsigned run(SpriteBuffer &screen);
};

#endif