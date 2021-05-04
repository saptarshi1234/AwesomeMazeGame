#include <SDL2/SDL.h>

#include <iostream>

#include "application.hpp"
#include "client.hpp"
#include "game.hpp"
#include "params.hpp"
#include "server.hpp"
#include "window.hpp"

using std::cout;
using std::endl;

const char* window_name = "GAME";

int main(int argc, char* argv[]) {
  bool isServer = true;
  char* ip = "127.0.0.1";

  cout << argc << endl;
  if (argc > 1) {
    cout << argv[1] << endl;
    if (strcmp(argv[1], "-c") == 0) {
      isServer = false;
      if (argc == 2) {
        ip = argv[2];
      }
    }
  }

  if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
    std::cout << "could not iniitialize SDL";
  std::cout << "SDL loaded" << std::endl;

  WindowManager window(window_name, Params::SCREEN_HEIGHT,
                       Params::SCREEN_WIDTH);
  Game game(window, isServer, ip);

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