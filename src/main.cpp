#include "game/game.hpp"
#include "game/globals.hpp"

int main()
    {
        game game;
        game.start();
        game.cleanup();

        return 0;
    }