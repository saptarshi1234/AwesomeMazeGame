#include "window.hpp"

#include <iostream>

#include "textures.hpp"

WindowManager::WindowManager() {}

WindowManager::WindowManager(const char* title, int h, int w) {
  window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
  SDL_Delay(200);
  if (window == NULL)
    std::cout << "Window failed to init. Error: " << SDL_GetError()
              << std::endl;

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  font32 = TTF_OpenFont("res/fonts/cocogoose.ttf", 32);
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

  if (layer.vertical_crop) {
    return {0, index * rect.h / total, rect.w, rect.h / total};
  }
  return {index * rect.w / total, 0, rect.w / total, rect.h};
}

SDL_Rect getDst(LayerDetails layer, SDL_Rect dst) {
  if (layer.dstSize.first != -1) {
    dst.w = layer.dstSize.first;
    dst.h = layer.dstSize.second;
  }
  return dst;
}

void WindowManager::render(SDL_Rect dst, SDL_Rect src, SDL_Texture* tex) {
  SDL_RenderCopy(renderer, tex, &src, &dst);
}

void WindowManager::renderText(std::pair<int, int> loc, const char* p_text,
                               TTF_Font* font, SDL_Color textColor) {
  auto [p_x, p_y] = loc;
  SDL_Surface* surfaceMessage = TTF_RenderText_Blended(font, p_text, textColor);
  SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

  SDL_Rect src;
  src.x = 0;
  src.y = 0;
  src.w = surfaceMessage->w;
  src.h = surfaceMessage->h;

  SDL_Rect dst;
  dst.x = p_x;
  dst.y = p_y;
  dst.w = src.w;
  dst.h = src.h;

  SDL_RenderCopy(renderer, message, &src, &dst);
  SDL_FreeSurface(surfaceMessage);
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

void WindowManager::renderPlayerDetails(Player& p) {
  SDL_Color white = {255, 255, 255};
  SDL_Color black = {0, 0, 0};
  int score = p.getScore();
  renderText({65, 23}, std::to_string(score).c_str(), font32, white);

  LayerDetails bar = {TextureManager::getTex(TextureID::HEALTH_BAR)};
  LayerDetails body = {TextureManager::getTex(TextureID::HEALTH_BODY)};

  int w = Params::WIDTH_OFFSET * 4 / 5;
  int h = Params::ACTUAL_CELL_SIZE / 4;
  int x = Params::SCREEN_WIDTH + (Params::WIDTH_OFFSET - w) / 2;
  int y = 50;

  auto collectedItems = p.getCollectedItems();
  for (int key = 0; key < collectedItems.size(); key++) {
    int status = collectedItems[key];
    if (status == -1) continue;

    int yy = y + key * 10;
    int ww = w * status / Params::POWERUP_TIME;

    render({x, yy, w, h}, bar.getSize(), bar.tex);
    render({x, yy, ww, h}, body.getSize(), body.tex);
  }
}

void WindowManager::display() { SDL_RenderPresent(renderer); }