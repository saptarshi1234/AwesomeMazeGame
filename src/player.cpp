#include "player.hpp"

#include <iostream>

#include "bullet.hpp"

Bullet Player::fireBullet(SDL_Texture* tex) {
  Bullet b;
  int x = location.x + location.w / 2;
  int y = location.y + location.h / 2;
  b.init({x, y, 2, 2}, tex);
  b.setDirection(this->dir);
  // std::cout << "hello" << std::endl;
  return b;
}

SDL_Rect Player::getCropArea() {
  auto rect = Entity::getCropArea();
  int index = 0;
  switch (dir) {
    case TOP:
      index = 1;
      break;
    case LEFT:
      index = 2;
      break;
    case RIGHT:
      index = 0;
      break;
    case BOTTOM:
      index = 3;
      break;
  }
  return {0, index * rect.h / 4, rect.w, rect.h / 4};
}