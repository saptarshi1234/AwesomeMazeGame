#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "entity.hpp"
#include "player.hpp"

class WindowManager {
 public:
  WindowManager();
  WindowManager(const char* w_title, int w_height, int w_width);
  void render(SDL_Rect loc, SDL_Rect dst, SDL_Texture* tex);
  void render(Entity& e);
  void renderText(std::pair<int, int> loc, const char* p_text, TTF_Font* font,
                  SDL_Color textColor);
  void renderCenter(std::pair<int, int>, const char* p_text, TTF_Font* font,
                    SDL_Color textColor);
  void displayText(int x, int y, int offset, int index, bool sound);
  void displayExitOptions(int x, int y, int offset, int index);
  void renderPlayerDetails(Player& p1, Player& p2, bool single);
  void displayFinalScore(int x, int y, int offset_x, int offset_y, int score1,
                         int score2, bool single_player);
  void displayIP(int x, int y, int offset_x, int offset_y, std::string ip,
                 bool wrong, bool connect_err);
  void clearWindow();
  void destroy();
  SDL_Texture* loadTexture(char* file);
  void display();

 private:
  SDL_Window* window;
  SDL_Renderer* renderer;
  TTF_Font* font32;
  TTF_Font* font16;
};