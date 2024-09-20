#ifndef BARS_HPP
#define BARS_HPP

#include "../ASCII_Engine/ObjetoDeJogo.hpp"
#include "../ASCII_Engine/Core/Cores_base.hpp"

#include <iostream>

using namespace std;

class Bars {
    private:
        ObjetoDeJogo* barra[63];
        int posL, posC, valMax;
        int animationAntIndex = 63;
    
    public:
        Bars(const int& posL, const int& posC, const int& val, COR::Cor cor = COR::PADRAO) : posL(posL), posC(posC), valMax(val) {
            for (int i = 0; i < 63; i++) {
                barra[i] = new ObjetoDeJogo("Barra", SpriteAnimado("rsc/SpritesAnimados/Bar.anm", 1, cor), posL-1, posC-1 + i);
            }
        }
        virtual ~Bars() {}

        void update(const int& val) {
            int activeBars = ((val * 63) / valMax);

            if (activeBars > 63) {
                activeBars = 63;
            } else if (activeBars <= 0) {   
                activeBars = 0;
            }

            for (int i = 0; i < 63; i++) {
                if (i < activeBars) {
                    barra[i]->ativarObj();
                } else {
                    barra[i]->desativarObj();
                }
            }

            if (activeBars > 0) {
                if (activeBars != animationAntIndex) {
                    barra[animationAntIndex - 1]->update();
                    barra[activeBars - 1]->update();
                    animationAntIndex = activeBars;
                }
            }
        }

        void draw(SpriteBase &screen) {
            for (int i = 0; i < 63; i++) {
                barra[i]->draw(screen, barra[i]->getPosL(), barra[i]->getPosC());
            }
        }
};

#endif // BARS_HPP