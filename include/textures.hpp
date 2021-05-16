#pragma once
#include <map>
#include <vector>

#include "window.hpp"

enum TextureID {
  TANK_F1,
  TANK_F2,
  BULLET,
  GROUND,
  GUN,
  EXPLOSION,
  CLIENT,
  SERVER,
  LOGO,
  HEALTH_BAR,
  HEALTH_BODY
};

class TextureManager {
 public:
  static std::map<TextureID, SDL_Texture*> tex_map;
  static void initialize(WindowManager win);
  static SDL_Texture* getTex(TextureID tex);
};  // namespace TextureManager