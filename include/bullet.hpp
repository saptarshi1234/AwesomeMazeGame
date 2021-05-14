#pragma once
#include "entity.hpp"
class Bullet : public Entity {
 public:
  void init(SDL_Rect loc);
  void setDirection(Direction dir);
  // void move(Maze *m);
};