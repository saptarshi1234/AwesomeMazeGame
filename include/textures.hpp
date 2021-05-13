#pragma once
#include <map>

#include "window.hpp"

enum Textures { PLAYER, BULLET, GROUND };

class TextureManager {
 private:
  std::map<Textures, SDL_Texture*> tex_map;

 public:
  void initialize(WindowManager win);
  SDL_Texture* getTex(Textures tex);
};