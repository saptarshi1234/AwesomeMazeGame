#include "bullet.hpp"

#include "player.hpp"
#include "textures.hpp"

void Bullet::init(SDL_Rect loc, bool fired_by, Player *player, double p) {
  physicalLocation = loc;

  int xx = loc.x + loc.w / 2 - loc.w / 10;
  int yy = loc.y + loc.h / 2 - loc.h / 10;
  SDL_Rect loc2 = {xx, yy, loc.w / 5, loc.h / 5};

  Entity::init(loc2);

  firedByPlayer = fired_by;
  shooter = player;
  power = p * Params::BULLET_POWER_UNIT;
  velocity = 15;
  layers.push_back({TextureManager::getTex(TextureID::BULLET), {0, 0}});
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

SDL_Rect Bullet::getPhysicalLocation() {
  return {location.x - physicalLocation.w / 2 + physicalLocation.w / 10,
          location.y - physicalLocation.h / 2 + physicalLocation.h / 10,
          physicalLocation.w, physicalLocation.h};
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

// std::string Bullet::to_string(Player *p1, Player *p2) {
//   char space = ' ';
//   std::stringstream ss;
//   auto loc = physicalLocation;

//   ss << loc.x << space << loc.y << space << dir << space << firedByPlayer
//      << space << !(p1 == shooter) << space << power;
//   return ss.str();
// }

// void Bullet::create_from_string(std::string s, Player *p1, Player *p2) {
//   int int_dir;
//   bool is_shooter;

//   std::stringstream ss(s);
//   ss >> location.x;
//   ss >> location.y;

//   init({location.x, location.y, p1->getLocation().w, p1->getLocation().h},
//        false, p1, 0);

//   ss >> int_dir;
//   ss >> firedByPlayer;
//   ss >> is_shooter;
//   ss >> power;

//   shooter = is_shooter ? p1 : p2;
//   setDirection(Direction(int_dir));
// }