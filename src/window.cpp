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

SDL_Rect getSrc(LayerDetails layer, Direction dir) {
  SDL_Rect rect = layer.getSize();

  if (layer.crop_details.second == -1) {
    int index = 0;
    switch (dir) {
      case TOP:
        index = 0;
        break;
      case RIGHT:
        index = 1;
        break;
      case BOTTOM:
        index = 2;
        break;
      case LEFT:
        index = 3;
        break;
    }
    return {0, index * rect.h / 4, rect.w, rect.h / 4};
  }
  auto [index, total] = layer.crop_details;
  return {0, index * rect.h / total, rect.w, rect.h / total};
}

SDL_Rect getDst(LayerDetails layer, SDL_Rect dst) {
  if (layer.dstSize.first != -1) {
    dst.w = layer.dstSize.first;
    dst.h = layer.dstSize.second;
  }
  return dst;
}

void WindowManager::render(Entity& entity) {
  for (auto layer : entity.getLayers()) {
    if (!layer.toShow) continue;

    SDL_Rect src = getSrc(layer, entity.getDirection());
    SDL_Rect dst = getDst(layer, entity.getPhysicalLocation());
    dst.x += layer.offset.first;
    dst.y += layer.offset.second;

    render(dst, src, layer.tex);
  }
}

void WindowManager::display() { SDL_RenderPresent(renderer); }