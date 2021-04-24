#include <SDL2/SDL.h>

#include <iostream>

#include "application.hpp"
#include "game.hpp"
#include "params.hpp"
#include "window.hpp"

const char* window_name = "GAME";

int main(int argc, char* args[]) {
  if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
    std::cout << "could not iniitialize SDL";
  std::cout << "SDL loaded" << std::endl;

  WindowManager window(window_name, Params::SCREEN_HEIGHT,
                       Params::SCREEN_WIDTH);
  Game game(window);

  srand((unsigned int)time(NULL));
  game.initialize();
  game.loadGame();

  while (game.isRunning()) {
    game.handleEvents();
    if (!game.isRunning()) break;

    game.update();
    window.clearWindow();

    game.render();
    window.display();

    game.wait();
  }

  game.quit();
  SDL_Quit();

  return 0;
}