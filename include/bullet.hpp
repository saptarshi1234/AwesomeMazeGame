#pragma once
#include "entity.hpp"
class Bullet : public Entity {
 public:
  void init(SDL_Rect loc, SDL_Texture* tex);
};