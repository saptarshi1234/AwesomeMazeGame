#pragma once

#include "bullet.hpp"
#include "entity.hpp"
class Player : public Entity {
 public:
  Bullet fireBullet(SDL_Texture* tex);
  SDL_Rect getCropArea();
};