#include "bullet.hpp"

void Bullet::init(SDL_Rect loc, SDL_Texture* tex) {
  Entity::init(loc, tex);
  velocity = 15;
  dir = RIGHT;
}
