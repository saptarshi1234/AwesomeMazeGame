#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

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
  TTF_Init();
  std::cout << "SDL loaded" << std::endl;

  WindowManager window(window_name, Params::SCREEN_HEIGHT,
                       Params::TOTAL_SCREEN_WIDTH);
  TextureManager::initialize(window);

  Entity splash_screen;
  splash_screen.init({0, 0, Params::TOTAL_SCREEN_WIDTH, Params::SCREEN_HEIGHT});
  // LayerDetails server =
  // splash_screen.setLayers(
  //     {{},
  //      {TextureManager::getTex(TextureID::SERVER), {0, 0}, {0, 1}}});

  LayerDetails logo = {TextureManager::getTex(TextureID::LOGO)};
  LayerDetails server = {TextureManager::getTex(TextureID::SERVER)};
  LayerDetails client = {TextureManager::getTex(TextureID::CLIENT)};

  SDL_Rect logo_loc = {0, 0, Params::TOTAL_SCREEN_WIDTH, Params::SCREEN_HEIGHT};
  SDL_Rect server_loc = server.getSize();
  SDL_Rect client_loc = client.getSize();

  server_loc.x = (logo_loc.w - server_loc.w) / 2;
  client_loc.x = (logo_loc.w - client_loc.w) / 2;

  server_loc.y = 5 * logo_loc.h / 8;
  client_loc.y = server_loc.y + 1.2 * server_loc.h;

  bool received_key = false;

  while (!received_key) {
    window.render(logo_loc, logo.getSize(), logo.tex);
    window.render(server_loc, server.getSize(), server.tex);
    window.render(client_loc, client.getSize(), client.tex);

    // window.render({0, 0, Params::SCREEN_WIDTH, Params::SCREEN_HEIGHT}, )
    window.display();
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        SDL_Quit();
        break;
      }

      if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
          case SDLK_s:
            isServer = true;
            received_key = true;
            break;
          case SDLK_c:
            isServer = false;
            received_key = true;
            break;
        }
      }
      // SDL_Delay(100);
    }
    window.clearWindow();
  }
  cout << isServer << endl;
  Game game(window, isServer, ip);

  srand((unsigned int)time(NULL));
  game.initialize();
  game.loadGame();

  int index = 0, update_freq = 50;
  Clock sleep_clk, stats_clk;

  window.clearWindow();
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
