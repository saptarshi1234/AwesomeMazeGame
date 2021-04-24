#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "entity.hpp"

class WindowManager {
 public:
  WindowManager();
  WindowManager(const char* w_title, int w_height, int w_width);
  void render(SDL_Rect loc, SDL_Texture* tex);
  void render(Entity& e);
  void clearWindow();
  void destroy();
  SDL_Texture* loadTexture(char* file);
  void display();

 private:
  SDL_Window* window;
  SDL_Renderer* renderer;
};