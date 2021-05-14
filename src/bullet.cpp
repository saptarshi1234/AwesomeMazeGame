#include "bullet.hpp"

#include "textures.hpp"

void Bullet::init(SDL_Rect loc) {
  Entity::init(loc);
  velocity = 15;
  layers.push_back({TextureManager::getTex(TextureID::BULLET), {0, 0}});
  // layers.push_back({TextureManager::getTex(TextureID::BULLET), {0, 0}});
}

void Bullet::setDirection(Direction dir) {
  Entity::setDirection(dir);
  int shift = 5;
  switch (dir) {
    case TOP:
      location.y -= shift;
      break;
    case RIGHT:
      location.x += shift;
      break;
    case BOTTOM:
      location.y += shift;
      break;
    case LEFT:
      location.x -= shift;
      break;
    default:
      break;
  }
}
