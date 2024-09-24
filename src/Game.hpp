#ifndef GAME_HPP
#define GAME_HPP

#include "Core/Player.hpp"
#include "Fases/Fase1.hpp"
#include "Fases/Fase2.hpp"
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
		FaseDois fase2("Fase 2", Sprite("rsc/Fases/Hud.img"), new Map("rsc/Fases/Map2.img", 2, 4, 2, 4));

		fase1.init();
		int result = fase1.run(buffer);
		//int result = Fase::LEVEL_COMPLETE;

		if (result == Fase::LEVEL_COMPLETE) {
			buffer.clear();
			fase2.init();
			result = fase2.run(buffer);

		}
    }
};

#endif