#include "player.hpp"

#include <iostream>

#include "bullet.hpp"

Bullet Player::fireBullet(SDL_Texture* tex) {
  Bullet b;
  b.init(location, tex);
  b.setDirection(this->dir);
  // std::cout << "hello" << std::endl;
  return b;
}

SDL_Rect Player::getCropArea() {
  auto rect = Entity::getCropArea();
  int index = 0;
  switch (dir) {
    case TOP:
      index = 0;
      break;
    case LEFT:
      index = 2;
      break;
    case RIGHT:
      index = 1;
      break;
    case BOTTOM:
      index = 3;
      break;
  }
  int tank_index = (moves % 8) / 4;
  return {tank_index * rect.w / 2, index * rect.h / 4, rect.w / 2, rect.h / 4};
}