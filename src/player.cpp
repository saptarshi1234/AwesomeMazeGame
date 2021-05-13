#include "player.hpp"

#include <iostream>

#include "bullet.hpp"
#include "textures.hpp"

void Player::init(SDL_Rect loc) {
  Entity::init(loc);
  F1_tex = TextureManager::getTex(TextureID::TANK_F1);
  F2_tex = TextureManager::getTex(TextureID::TANK_F2);
  layers = {{F1_tex, {0, 0}}, {TextureManager::getTex(TextureID::GUN), {0, 0}}};
}

Bullet Player::fireBullet() {
  Bullet b;
  b.init(location);
  b.setDirection(this->dir);
  firing = 1;
  switch (dir) {
    case TOP:
      layers[1].offset = {0, 5};
      break;
    case RIGHT:
      layers[1].offset = {-5, 0};
      break;
    case BOTTOM:
      layers[1].offset = {0, -5};
      break;
    case LEFT:
      layers[1].offset = {5, 0};
      break;
    default:
      break;
  }
  return b;
}

void Player::move(Maze* maze) {
  Entity::move(maze);
  if (firing > 0) firing++;
  if (firing == 4) {
    layers[1].offset = {0, 0};
    firing = 0;
  }
}
