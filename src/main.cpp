#include <SDL2/SDL.h>

#include <iostream>

#include "game.hpp"
#include "params.hpp"
#include "window.hpp"

using std::cout;
using std::endl;

const char* window_name = "GAME";

struct Clock {
  int last_tick_time = 0;
  int delta = 0;

  void tick() {
    int tick_time = SDL_GetTicks();
    delta = tick_time - last_tick_time;
    last_tick_time = tick_time;
  }
};

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
  TextureManager::initialize(window);

  Game game(window, isServer, ip);

  srand((unsigned int)time(NULL));
  game.initialize();
  game.loadGame();

  int index = 0, update_freq = 50;
  Clock sleep_clk, stats_clk;

  while (game.isRunning()) {
    sleep_clk.tick();
    stats_clk.tick();

    game.handleEvents();
    if (!game.isRunning()) break;

    game.update();
    game.sync();
    window.clearWindow();

    game.render();
    window.display();

    sleep_clk.tick();
    game.wait(std::max(0, 60 - sleep_clk.delta));

    cout << stats_clk.delta << endl;
  }

  game.quit();
  SDL_Quit();

  return 0;
}

// TODO

// Akshat
// * bot AI -
// * better movements -
// * bot dies - point table

// Sap
// * make clean
// * sprite sheets and animation
// * startup screen with choices
// * powerups
