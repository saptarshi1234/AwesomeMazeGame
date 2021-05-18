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
  void renderPlayerDetails(Player& p);
  void clearWindow();
  void destroy();
  SDL_Texture* loadTexture(char* file);
  void display();

 private:
  SDL_Window* window;
  SDL_Renderer* renderer;
  TTF_Font* font32;
};