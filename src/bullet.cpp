#include "bullet.hpp"

#include "player.hpp"
#include "textures.hpp"

void Bullet::init(SDL_Rect loc, bool fired_by, Player *player, double p) {
  Entity::init(loc);
  firedByPlayer = fired_by;
  shooter = player;
  power = p * Params::BULLET_POWER_UNIT;
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

bool Bullet::destroyBullet() { return destroy; }

bool Bullet::hitTarget(Player &p) {
  if (&p == shooter) {
    return false;
  }
  if (p.isBot() && !firedByPlayer) {
    return false;
  }
  Direction dir = Entity::getDirection();
  SDL_Rect p_loc = p.getLocation();
  int x = location.x + (location.w / 2), y = location.y + (location.h / 2);
  bool hit = false;
  int final_x = location.x, final_y = location.y;
  switch (dir) {
    case TOP:
      if (x >= p_loc.x && x < p_loc.x + p_loc.w && y >= p_loc.y &&
          y < p_loc.y + p_loc.h + velocity) {
        final_y = p_loc.y + (2 * p_loc.h - location.h) / 2;
        hit = true;
      }
      break;
    case RIGHT:
      if (x >= p_loc.x - velocity && x < p_loc.x + p_loc.w && y >= p_loc.y &&
          y < p_loc.y + p_loc.h) {
        final_x = p_loc.x - (location.w) / 2;
        hit = true;
      }
      break;
    case BOTTOM:
      if (x >= p_loc.x && x < p_loc.x + p_loc.w && y >= p_loc.y - velocity &&
          y < p_loc.y + p_loc.h) {
        final_y = p_loc.y - (location.h) / 2;
        hit = true;
      }
      break;
    case LEFT:
      if (x >= p_loc.x && x < p_loc.x + p_loc.w + velocity && y >= p_loc.y &&
          y < p_loc.y + p_loc.h) {
        final_x = p_loc.x + (2 * p_loc.w - location.w) / 2;
        hit = true;
      }
      break;
    default:
      break;
  }
  if (!hit) {
    return false;
  }
  destroy = true;
  if (p.getHP() <= power) {
    p.explosion_status = 1;
    p.setHP(0);
    if (firedByPlayer) shooter->raiseScore(100);

  } else {
    p.setHP(p.getHP() - power);
    if (firedByPlayer) shooter->raiseScore(50);
  }
  if (!(x >= p_loc.x && x < p_loc.x + p_loc.w && y >= p_loc.y &&
        y < p_loc.y + p_loc.h)) {
    location.x = final_x;
    location.y = final_y;
  }
  return true;
}
