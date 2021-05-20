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

const char* window_name = "Tank Wars";

struct Clock {
  int last_tick_time = 0;
  int delta = 0;
  int initial;
  int started = false;

  void tick() {
    int tick_time = SDL_GetTicks();
    if (!started) {
      started = true;
      initial = tick_time;
    }
    delta = tick_time - last_tick_time;
    last_tick_time = tick_time;
  }

  int getElapsed() { return SDL_GetTicks() - initial; }
};

int main(int argc, char* argv[]) {
  bool isServer = true;
  bool single_player = false;

  const char* ip = "127.0.0.1";

  if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
    std::cout << "could not iniitialize SDL";
  TTF_Init();
  std::cout << "SDL loaded" << std::endl;
  Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
  SoundManager::initialize();

  srand((unsigned)time(0));
  bool sound = true;
  WindowManager window(window_name, Params::SCREEN_HEIGHT,
                       Params::TOTAL_SCREEN_WIDTH);
  TextureManager::initialize(window);

  TTF_Font* font_large = TTF_OpenFont("res/fonts/cocogoose.ttf", 36);
  TTF_Font* font_small = TTF_OpenFont("res/fonts/cocogoose.ttf", 24);

  bool quit = false;
  while (!quit) {
    Entity splash_screen;
    splash_screen.init(
        {0, 0, Params::TOTAL_SCREEN_WIDTH, Params::SCREEN_HEIGHT});

    LayerDetails logo = {TextureManager::getTex(TextureID::LOGO)};
    LayerDetails server = {TextureManager::getTex(TextureID::SERVER)};
    LayerDetails client = {TextureManager::getTex(TextureID::CLIENT)};

    SDL_Rect logo_loc = {0, 0, Params::TOTAL_SCREEN_WIDTH,
                         Params::SCREEN_HEIGHT};
    SDL_Rect server_loc = server.getSize();
    SDL_Rect client_loc = client.getSize();

    int x = logo_loc.x + (logo_loc.w - server_loc.w) / 2;
    int y1 = logo_loc.y + 5 * logo_loc.h / 8;
    int offset = server_loc.h * 0.5;

    bool received_key = false;
    int choice_index = 1;

    while (!received_key) {
      window.clearWindow();

      window.render(logo_loc, logo.getSize(), logo.tex);
      window.displayText(x, y1, offset, choice_index, sound);

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
              if (choice_index > 4) choice_index = 4;
              break;
            case SDLK_RETURN:
            case SDLK_RETURN2:
              single_player = choice_index == 1;
              isServer = choice_index == 2;
              if (choice_index == 4) {
                sound = !(sound);
              } else {
                received_key = true;
              }
              break;
          }
        }
      }
      SDL_Delay(100);
    }

    SDL_Rect overlay_loc = {0, 0, Params::TOTAL_SCREEN_WIDTH,
                            Params::SCREEN_HEIGHT};
    LayerDetails overlay = {TextureManager::getTex(TextureID::OVERLAY)};

    // window.clearWindow();
    if (!single_player) {
      std::string current_ip = "Current IP - " + CustomSocket::ip_details();

      window.render(overlay_loc, overlay.getSize(), overlay.tex);
      window.renderCenter({}, "Waiting for Client to join", font_large,
                          {255, 255, 255});
      window.renderCenter({0, 40}, current_ip.c_str(), font_small,
                          {255, 255, 255});
      window.display();
    }
    cout << "sound" << sound << endl;
    cout << "single player" << single_player << endl;
    cout << "server" << isServer << endl;

    SoundManager::sound_on = sound;

    Game game;
    if (!single_player && !isServer) {
      Entity ip_screen;
      ip_screen.init({0, 0, Params::TOTAL_SCREEN_WIDTH, Params::SCREEN_HEIGHT});
      LayerDetails id_bg = {TextureManager::getTex(TextureID::IP)};

      SDL_Rect loc = ip_screen.getLocation();
      x = loc.x + (loc.w) * 3 / 20;
      int offset_x = loc.w / 10;
      int y = loc.y + loc.h / 8;
      int offset_y = 2 * loc.h / 7;

      std::string input_ip = "";
      std::string wrong_ip;
      bool check_wrong_ip = false, connect_err = false;

      received_key = false;
      SDL_StartTextInput();
      while (!received_key) {
        window.clearWindow();
        window.render(loc, id_bg.getSize(), id_bg.tex);
        if (check_wrong_ip) {
          window.displayIP(x, y, offset_x, offset_y, input_ip,
                           wrong_ip == input_ip, connect_err);
          if (input_ip != wrong_ip) {
            check_wrong_ip = false;
          }
        } else {
          window.displayIP(x, y, offset_x, offset_y, input_ip, false,
                           connect_err);
        }

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
              case SDLK_RETURN:
              case SDLK_RETURN2:
                if (input_ip == "1") {
                  input_ip = "127.0.0.1";
                }
                try {
                  game =
                      Game(window, isServer, single_player, input_ip.c_str());
                  received_key = true;
                } catch (InvalidIP e) {
                  check_wrong_ip = true;
                  wrong_ip = input_ip;
                } catch (ConnectionError e) {
                  connect_err = true;
                }
                break;
              case SDLK_BACKSPACE:
                if (input_ip.length() > 0) {
                  input_ip.pop_back();
                }
                break;
              case SDLK_c:
                if (SDL_GetModState() & KMOD_CTRL) {
                  SDL_SetClipboardText(input_ip.c_str());
                }
                break;
              case SDLK_v:
                if (SDL_GetModState() & KMOD_CTRL) {
                  input_ip = SDL_GetClipboardText();
                }
                break;
            }
          } else if (event.type == SDL_TEXTINPUT) {
            if (!(SDL_GetModState() & KMOD_CTRL &&
                  (event.text.text[0] == 'c' || event.text.text[0] == 'C' ||
                   event.text.text[0] == 'v' || event.text.text[0] == 'V'))) {
              input_ip += event.text.text;
            }
          }
        }
        SDL_Delay(100);
      }
      SDL_StopTextInput();
    } else {
      game = Game(window, isServer, single_player, ip);
    }

    srand((unsigned int)time(NULL));
    game.initialize();
    game.loadGame();

    bool disconnected = false;
    int index = 0, update_freq = 50;
    Clock sleep_clk, stats_clk;

    window.clearWindow();
    while (game.isRunning()) {
      sleep_clk.tick();
      stats_clk.tick();

      game.handleEvents();
      if (!game.isRunning()) break;

      try {
        game.updateBots();
        game.sync();
        game.update();
      } catch (DisconnectError e) {
        disconnected = true;
        break;
      }

      game.modifyTime(stats_clk.getElapsed());

      window.clearWindow();

      game.render();
      window.display();

      sleep_clk.tick();
      game.wait(std::max(0, 60 - sleep_clk.delta));

      cout << stats_clk.delta << endl;
    }
    int score1 = 0;
    int score2 = 0;
    if (single_player) {
      score1 = game.player1.getScore();
    } else {
      score1 = game.player1.getScore();
      score2 = game.player2.getScore();
    }
    cout << score1 << " " << score2;
    Entity score_screen;
    score_screen.init(
        {Params::TOTAL_SCREEN_WIDTH / 8, Params::SCREEN_HEIGHT / 8,
         6 * Params::TOTAL_SCREEN_WIDTH / 8, 6 * Params::SCREEN_HEIGHT / 8});

    LayerDetails score = {TextureManager::getTex(TextureID::SCORE)};
    // LayerDetails overlay = {TextureManager::getTex(TextureID::OVERLAY)};

    SDL_Rect loc = score_screen.getLocation();
    x = loc.x + (loc.w) * 6 / 20;
    int offset_x = loc.w / 10;
    int y = loc.y + loc.h / 8;
    int offset_y = loc.h / 7;

    received_key = false;
    SDL_Rect TOTAL_SCREEN_SIZE = {0, 0, Params::TOTAL_SCREEN_WIDTH,
                                  Params::SCREEN_HEIGHT};

    while (!received_key) {
      if (disconnected) {
        SDL_Color white = {255, 255, 255};
        window.render(TOTAL_SCREEN_SIZE, overlay.getSize(), overlay.tex);
        window.renderCenter({}, "Lost connection with player", font_large,
                            white);
        window.renderText({x + 24 * offset_x / 5, y + 11 * offset_y / 2},
                          "PRESS ENTER", font_small, white);

      } else {
        window.render(loc, score.getSize(), score.tex);
        window.displayFinalScore(x, y, offset_x, offset_y, score1, score2,
                                 single_player);
      }

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
            case SDLK_RETURN:
            case SDLK_RETURN2:
              received_key = true;
              break;
          }
        }
      }
      SDL_Delay(100);
    }

    Entity exit_screen;
    exit_screen.init({0, 0, Params::TOTAL_SCREEN_WIDTH, Params::SCREEN_HEIGHT});

    logo_loc = exit_screen.getLocation();
    x = logo_loc.x + 4 * (logo_loc.w) / 10;
    y1 = logo_loc.y + 5 * logo_loc.h / 8;
    offset = logo_loc.h / 12;

    received_key = false;
    choice_index = 1;
    bool restart = false;
    bool quit_game = false;
    while (!received_key) {
      window.clearWindow();
      window.render(logo_loc, logo.getSize(), logo.tex);
      window.displayExitOptions(x, y1, offset, choice_index);

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
              if (choice_index > 2) choice_index = 2;
              break;
            case SDLK_RETURN:
            case SDLK_RETURN2:
              restart = choice_index == 1;
              quit_game = choice_index == 2;
              received_key = true;
              break;
          }
        }
      }
      SDL_Delay(100);
    }
    if (quit_game) break;
  }
  window.destroy();
  SDL_Quit();
  TTF_Quit();

  return 0;
}
