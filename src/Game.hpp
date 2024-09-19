#ifndef GAME_HPP
#define GAME_HPP

#include "ASCII_Engine/SpriteBuffer.hpp"
#include "Fases/Fase1.hpp"
#include "Core/Map.hpp"

using namespace std;

class Game {
public:
	Game(){}
	~Game(){}
	
	static void run()
	{
		SpriteBuffer buffer(338,74);

		FaseUm fase1("Fase 1", Sprite("rsc/Fases/Hud.img"), new Map("rsc/Fases/Map1.img", 2, 4, 2, 4));

		fase1.init();
		fase1.run(buffer);
    }
};

#endif