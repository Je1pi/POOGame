#include "GameResult.hpp"

unsigned GameResult::run(SpriteBuffer &screen)
{
    draw(screen);
	system("clear");
	show(screen);

    return Fase::END_GAME;
}