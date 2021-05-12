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

void WindowManager::render(SDL_Rect dst, SDL_Rect src, SDL_Texture* tex) {
  SDL_RenderCopy(renderer, tex, &src, &dst);
}

void WindowManager::render(Entity& entity) {
  SDL_Rect src = entity.getCropArea();
  SDL_Rect dst = entity.getLocation();

  render(dst, src, entity.getTexture());
}

void WindowManager::display() { SDL_RenderPresent(renderer); }