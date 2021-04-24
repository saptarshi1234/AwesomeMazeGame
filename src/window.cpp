#include "window.hpp"

#include <iostream>

WindowManager::WindowManager() {}

WindowManager::WindowManager(const char* title, int h, int w) {
  window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
  SDL_Delay(200);
  if (window == NULL)
    std::cout << "Window failed to init. Error: " << SDL_GetError()
              << std::endl;

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

SDL_Texture* WindowManager::loadTexture(char* file) {
  SDL_Texture* texture = NULL;
  texture = IMG_LoadTexture(renderer, file);

  if (texture == NULL)
    std::cout << "Failed to load texture. Error: " << SDL_GetError()
              << std::endl;

  return texture;
}
void WindowManager::destroy() { SDL_DestroyWindow(window); }

void WindowManager::clearWindow() { SDL_RenderClear(renderer); }

void WindowManager::render(SDL_Rect loc, SDL_Texture* tex) {
  SDL_Rect src;
  src.x = 0;
  src.y = 0;
  SDL_QueryTexture(tex, NULL, NULL, &src.w, &src.h);

  SDL_Rect dst = loc;

  SDL_RenderCopy(renderer, tex, &src, &dst);
}

void WindowManager::render(Entity& entity) {
  SDL_Rect src = entity.getLocation();
  SDL_Rect dst = entity.getLocation();

  SDL_Rect loc = entity.getLocation();
  render(loc, entity.getTexture());

  // dst.x = entity.getX() + p_entity.getAnimOffsetX(i);
  // dst.y = entity.getY() + p_entity.getAnimOffsetY(i);
  // dst.w = entity.getCurrentFrame().w;
  // dst.h = entity.getCurrentFrame().h;
  // SDL_RenderCopy(renderer, entity.getTexture(), &src, &dst);
}

void WindowManager::display() { SDL_RenderPresent(renderer); }