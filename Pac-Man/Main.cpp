#include <SFML/Graphics.hpp>
#include "Game.h"

int main()
{
    Game game;
    while (!game.isFinished()) {
        game.processInput();
        if (!game.isPaused()) game.update();
        game.render();
        game.clockRestart();
    }
    game.saveHighScore();

    return 0;
}