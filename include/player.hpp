#pragma once

#include "bullet.hpp"
#include "entity.hpp"
class Player : public Entity {
 protected:
  SDL_Texture *F1_tex;
  SDL_Texture *F2_tex;
  SDL_Texture *gun;
  SDL_Texture *explosion;
  int firing = 0;

 public:
  int explosion_status = 0;
  void init(SDL_Rect loc);
  Bullet fireBullet();
  void move(Maze *maze);
  // SDL_Rect getCropArea();
};