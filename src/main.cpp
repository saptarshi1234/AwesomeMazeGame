#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include <iostream>

#include "game.hpp"
#include "params.hpp"
#include "sounds.hpp"
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
  bool single_player = false;

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
  Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
  SoundManager::initialize();

  srand((unsigned)time(0));

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

  int x = (logo_loc.w - server_loc.w) / 2;
  int y1 = 5 * logo_loc.h / 8;
  int offset = server_loc.h * 0.5;

  bool received_key = false;
  int choice_index = 1;

  while (!received_key) {
    window.clearWindow();

    window.render(logo_loc, logo.getSize(), logo.tex);
    window.displayText(x, y1, offset, choice_index);

    window.display();
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        window.destroy();
        TTF_Quit();
        SDL_Quit();
        exit(0);
      }

      if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
          case SDLK_UP:
            choice_index--;
            if (choice_index < 1) choice_index = 1;
            break;
          case SDLK_DOWN:
            choice_index++;
            if (choice_index > 3) choice_index = 3;
            break;
          case SDLK_RETURN:
          case SDLK_RETURN2:
            single_player = choice_index == 1;
            isServer = choice_index == 2;
            received_key = true;
            break;
        }
      }
    }
    SDL_Delay(100);
  }
  cout << "single player" << single_player << endl;
  cout << "server" << isServer << endl;
  Game game(window, isServer, single_player, ip);

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

    game.updateBots();
    game.sync();
    game.update();

    window.clearWindow();

    game.render();
    window.display();

    sleep_clk.tick();
    game.wait(std::max(0, 60 - sleep_clk.delta));

    cout << stats_clk.delta << endl;
  }

  game.quit();
  SDL_Quit();
  TTF_Quit();

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
