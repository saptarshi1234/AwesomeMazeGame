#pragma once
#include <map>
#include <vector>

#include "window.hpp"

enum TextureID {
  TANK_F1,
  TANK_F2,
  TANK_INV,
  GUN,
  GUN_INV,
  BULLET,

  GROUND,
  EXPLOSION,

  CLIENT,
  SERVER,
  LOGO,

  HEALTH_BAR,
  HEALTH_BODY,

  MULTIPLIER_2X,
  INVISIBLE,
  POWERUP,
  SHIELD,

  SCORE
};

class TextureManager {
 public:
  static std::map<TextureID, SDL_Texture*> tex_map;
  static void initialize(WindowManager win);
  static SDL_Texture* getTex(TextureID tex);
};  // namespace TextureManager