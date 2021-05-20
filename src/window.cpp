#include "window.hpp"

#include <iostream>
#include <string>

#include "params.hpp"
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
  font16 = TTF_OpenFont("res/fonts/cocogoose.ttf", 16);
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
void WindowManager::displayText(int x, int y, int offset, int index,
                                bool sound) {
  TTF_Font* font = TTF_OpenFont("res/fonts/cocogoose.ttf", 24);
  SDL_Color white = {255, 255, 255};
  SDL_Color selected = {0, 255, 0};

  renderText({x, y}, "1 Player", font, index == 1 ? selected : white);
  renderText({x, y + offset}, "2 Player : Server", font,
             index == 2 ? selected : white);
  renderText({x, y + 2 * offset}, "2 Player : Client", font,
             index == 3 ? selected : white);
  renderText({x, y + 3 * offset}, sound ? "Sound: On" : "Sound: Off", font,
             index == 4 ? selected : white);

  TTF_CloseFont(font);
}

void WindowManager::displayExitOptions(int x, int y, int offset, int index) {
  TTF_Font* font = TTF_OpenFont("res/fonts/cocogoose.ttf", 24);
  SDL_Color white = {255, 255, 255};
  SDL_Color selected = {0, 255, 0};

  renderText({x, y}, "RESTART", font, index == 1 ? selected : white);
  renderText({x, y + offset}, "QUIT", font, index == 2 ? selected : white);

  TTF_CloseFont(font);
}

void WindowManager::displayFinalScore(int x, int y, int offset_x, int offset_y,
                                      int score1, int score2,
                                      bool single_player) {
  TTF_Font* head_font = TTF_OpenFont("res/fonts/cocogoose.ttf", 36);
  TTF_Font* font = TTF_OpenFont("res/fonts/cocogoose.ttf", 24);
  SDL_Color white = {255, 255, 255};
  SDL_Color winner = {0, 255, 0};
  SDL_Color loser = {255, 148, 148};
  SDL_Color game_over = {255, 0, 0};

  renderText({x, y}, "GAME OVER", head_font, game_over);

  renderText({x + offset_x, y + 3 * offset_y / 2}, "SCORES", font, white);

  renderText({x, y + 5 * offset_y / 2}, "YOU :", font, white);
  renderText({x + 5 * offset_x / 2, y + 5 * offset_y / 2},
             std::to_string(score1).c_str(), font, white);

  if (!single_player) {
    renderText({x, y + 3 * offset_y}, "THEY :", font, white);
    renderText({x + 5 * offset_x / 2, y + 3 * offset_y},
               std::to_string(score2).c_str(), font, white);
    if (score1 != score2) {
      renderText(
          {score1 > score2 ? x - offset_x / 2 : x - offset_x, y + 4 * offset_y},
          score1 > score2 ? "Yay! You Won!" : "Oh No! You Lost!", head_font,
          score1 > score2 ? winner : loser);
    } else {
      renderText({x + 6 * offset_x / 5, y + 4 * offset_y}, "TIE!", head_font,
                 winner);
    }
  }

  renderText({x + 24 * offset_x / 5, y + 11 * offset_y / 2}, "PRESS ENTER",
             font16, white);

  TTF_CloseFont(font);
  TTF_CloseFont(head_font);
}

void WindowManager::displayIP(int x, int y, int offset_x, int offset_y,
                              std::string ip, bool wrong, bool connect_err) {
  TTF_Font* head_font = TTF_OpenFont("res/fonts/cocogoose.ttf", 36);
  TTF_Font* font = TTF_OpenFont("res/fonts/cocogoose.ttf", 24);
  SDL_Color white = {255, 255, 255};
  SDL_Color green = {0, 255, 0};
  SDL_Color error = {255, 148, 148};
  SDL_Color red = {255, 0, 0};

  renderText({x, y}, "ENTER SERVER'S IP ADDRESS", head_font, red);

  renderText({x + 2 * offset_x, y + offset_y / 4}, "(Enter 1 for localhost)",
             font16, white);
  if (ip.length() > 0) {
    renderText({x + 2 * offset_x, y + offset_y}, ip.c_str(), font,
               wrong ? error : green);
  }
  if (wrong) {
    renderText({x + 11 * offset_x / 5, y + 7 * offset_y / 5},
               "[Invalid IP address]", font16, error);
  }
  if (connect_err) {
    renderText({x + 11 * offset_x / 5, y + 7 * offset_y / 5},
               "Failed to establish connection", font16, error);
  }
  TTF_CloseFont(font);
  TTF_CloseFont(head_font);
}

void WindowManager::renderPlayerDetails(Player& p1, Player& p2, bool single) {
  SDL_Color white = {255, 255, 255};
  SDL_Color black = {0, 0, 0};
  int score1 = p1.getScore();
  int x1 = Params::SCREEN_WIDTH + 10;
  int x2 = Params::SCREEN_WIDTH + Params::WIDTH_OFFSET / 2;
  int y1 = 10;
  int y2 = 30;
  renderText({x1, y1}, "You :", font16, white);
  renderText({x2, y1}, std::to_string(score1).c_str(), font16, white);

  if (!single) {
    int score2 = p2.getScore();
    renderText({x1, y2}, "They:", font16, white);
    renderText({x2, y2}, std::to_string(score2).c_str(), font16, white);
  }

  LayerDetails bar = {TextureManager::getTex(TextureID::HEALTH_BAR)};
  LayerDetails body = {TextureManager::getTex(TextureID::HEALTH_BODY)};

  int icon_size = 20;

  int w = (Params::WIDTH_OFFSET - icon_size) * 4 / 5;
  int h = Params::ACTUAL_CELL_SIZE / 4;
  int x = Params::SCREEN_WIDTH + icon_size +
          (Params::WIDTH_OFFSET - icon_size - w) / 2;
  int y = y2 + 50;

  auto collectedItems = p1.getCollectedItems();
  for (int key = 0; key < collectedItems.size(); key++) {
    int status = collectedItems[key];
    if (status == -1 || key == 4) continue;

    int yy = y + key * 30;
    int ww = w * status / Params::POWERUP_TIME;

    Item item;
    item.init({Params::SCREEN_WIDTH + 5, yy - 10, 20, 20}, Item::ItemType(key));

    render(item);

    render({x, yy, w, h}, bar.getSize(), bar.tex);
    render({x, yy, ww, h}, body.getSize(), body.tex);
  }
}

void WindowManager::display() { SDL_RenderPresent(renderer); }