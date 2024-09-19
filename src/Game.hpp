#ifndef GAME_HPP
#define GAME_HPP

#include "ASCII_Engine/SpriteBuffer.hpp"
#include "Fases/Fase1.hpp"

using namespace std;

class Game {
public:
	Game(){}
	~Game(){}
	
	static void run()
	{
		SpriteBuffer buffer(338,74);

		FaseUm fase1("Fase 1", Sprite("rsc/Fases/Hud.img"), Sprite("rsc/Fases/Map1.img"));

		fase1.init();
		fase1.run(buffer);
    }
};

#endif