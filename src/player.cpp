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