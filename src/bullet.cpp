#include "bullet.hpp"

void Bullet::init(SDL_Rect loc, SDL_Texture* tex) {
  Entity::init(loc, tex);
  velocity = 15;
  dir = RIGHT;
}

SDL_Rect Bullet::getCropArea() {
  auto rect = Entity::getCropArea();
  int index = 0;
  switch (dir) {
    case TOP:
      index = 0;
      break;
    case LEFT:
      index = 3;
      break;
    case RIGHT:
      index = 1;
      break;
    case BOTTOM:
      index = 2;
      break;
  }
  return {0, index * rect.h / 4, rect.w, rect.h / 4};
}