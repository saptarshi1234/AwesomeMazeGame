#include "player.hpp"

#include <cfloat>
#include <iostream>
#include <vector>

#include "bullet.hpp"
#include "items.hpp"
#include "textures.hpp"
using namespace std;
void Player::init(SDL_Rect loc) {
  for (int i = 0; i < Item::numTypes; i++) collected.push_back(-1);

  int w = 2 * Params::ACTUAL_CELL_SIZE * 8 / 10;
  int h = 2 * Params::ACTUAL_CELL_SIZE / 10;

  int offX = Params::ACTUAL_CELL_SIZE / 2;

  Entity::init(loc);
  F1_tex = TextureManager::getTex(TextureID::TANK_F1);
  F2_tex = TextureManager::getTex(TextureID::TANK_F2);
  gun = TextureManager::getTex(TextureID::GUN);
  layers = {
      {F1_tex, {0, 0}},
      {TextureManager::getTex(TextureID::GUN), {0, 0}},
      {TextureManager::getTex(TextureID::EXPLOSION), {0, 0}, {0, 8}, false},
      {TextureManager::getTex(TextureID::HEALTH_BAR),
       {-offX, -offX},
       {0, 1},
       true,
       {w, h}},
      {TextureManager::getTex(TextureID::HEALTH_BODY),
       {-offX, -offX},
       {0, 1},
       true,
       {w, h}}};
}

SDL_Rect Player::getPhysicalLocation() { return location; }

Bullet Player::fireBullet() {
  Bullet b;
  b.init(location, !is_bot, this, bullet_power);
  b.setDirection(this->dir);

  firing = 1;
  // int recoil = 5, shift = 5;
  // switch (dir) {
  //   case TOP:
  //     layers[1].offset = {0, recoil};
  //     break;
  //   case RIGHT:
  //     layers[1].offset = {-recoil, 0};
  //     break;
  //   case BOTTOM:
  //     layers[1].offset = {0, -recoil};
  //     break;
  //   case LEFT:
  //     layers[1].offset = {recoil, 0};
  //     break;
  //   default:
  //     break;
  // }

  bullet_fired = true;
  return b;
}

void Player::setHP(double h) {
  hp = h;
  layers[4].dstSize.first =
      (int)(Params::ACTUAL_CELL_SIZE * 16 / 10 * hp / Params::MAX_HP);
}
void Player::setBulletPower(double x) { bullet_power = x; }
void Player::raiseScore(int s) { score += s * score_multiplier; }
double Player::getHP() { return hp; }
double Player::getBulletPower() { return bullet_power; }
int Player::getScore() { return score; }
bool Player::isBot() { return is_bot; }

void Player::move(Maze* maze) {
  collided = false;
  if (explosion_status == 0) {
    Entity::move(maze);
  }
  if (firing > 0) firing++;
  if (firing == 4) {
    layers[1].offset = {0, 0};
    firing = 0;
  }

  if (explosion_status > 0) {
    layers[2].toShow = true;
    if (explosion_status == 2) {
      layers[0].toShow = false;
      layers[1].toShow = false;
    } else if (explosion_status == 1) {
      is_moving = false;
    }
    layers[2].crop_details.first = explosion_status - 1;
    explosion_status++;

    if (explosion_status == 8) {
      explosion_status = -1;
      // TODO replace
      layers[2].toShow = false;
    }
  }
}

void Player::updateState() {
  if (firing == 4) {
    layers[1].offset = {0, 0};
  }

  if (explosion_status > 0) {
    layers[2].toShow = true;
    if (explosion_status == 2) {
      layers[0].toShow = false;
      layers[1].toShow = false;

      layers[2].crop_details.first = explosion_status - 1;

      if (explosion_status == 8) {
        layers[2].toShow = false;
      }
    }
  }
}
void Player::collectItem(Item& item) {
  collected[item.getType()] = Params::POWERUP_TIME;
  switch (item.getType()) {
    case Item::ItemType::INVISIBLE:
      layers[0].tex = TextureManager::getTex(TextureID::TANK_INV);
      layers[1].tex = TextureManager::getTex(TextureID::GUN_INV);
      break;
    case Item::ItemType::MULTIPLIER:
      score_multiplier = 2;
      break;
    case Item::ItemType::POWERUP:
      this->bullet_power *= 2;
      break;
    case Item::ItemType::SHIELD:
      break;
  }
}

void Player::updateItems() {
  for (int key = 0; key <= collected.size(); key++) {
    if (collected[key] == -1) continue;
    collected[key]--;
    if (collected[key] == 0) {
      switch (key) {
        case Item::ItemType::INVISIBLE:
          layers[0].tex = F1_tex;
          layers[1].tex = gun;
          break;
        case Item::ItemType::MULTIPLIER:
          score_multiplier = 1;
          break;
        case Item::ItemType::POWERUP:
          this->bullet_power /= 2;
          break;
        case Item::ItemType::SHIELD:
          break;
      }
      collected[key] = -1;
    }
  }
}

std::vector<int> Player::getCollectedItems() { return collected; }

void Player::create_from_string(std::string s) {
  int int_dir;
  std::stringstream ss(s);
  ss >> location.x;
  ss >> location.y;
  ss >> location.h;
  ss >> location.w;
  ss >> is_moving;
  ss >> bullet_fired;
  ss >> explosion_status;
  ss >> hp;
  ss >> int_dir;

  dir = Direction(int_dir);
  // setDirection(Direction(int_dir));
}

void Player::update_from_string(std::string s) {
  std::stringstream ss(s);
  int int_dir;
  ss >> int_dir;
  ss >> int_dir;

  // dir = Direction(int_dir);
  setDirection(Direction(int_dir));
}

std::string Player::to_string() {
  std::stringstream ss;
  char space = ' ';
  ss << location.x << space << location.y << space << location.h << space
     << location.w << space << is_moving << space << bullet_fired << space
     << explosion_status << space << hp << space << dir;
  return ss.str();
}

std::string Player::to_update() {
  std::stringstream ss;
  ss << dir;
  return ss.str();
}

bool rayRectCollision(vector<double> point, vector<double> dir, SDL_FRect obj,
                      double& t, double& nx, double& ny) {
  double nearX, nearY, farX, farY;
  if (dir[1] != 0) {
    nearY = (obj.y - point[1]) / dir[1];
    farY = (obj.y + obj.h - point[1]) / dir[1];
  } else {
    if (obj.y - point[1] > 0)
      nearY = DBL_MAX;
    else if (obj.y - point[1] < 0)
      nearY = -DBL_MAX;
    else
      nearY = 0;
    if (obj.y + obj.h - point[1] > 0)
      farY = DBL_MAX;
    else if (obj.y + obj.h - point[1] < 0)
      farY = -DBL_MAX;
    else
      farY = 0;
  }
  if (dir[0] != 0) {
    nearX = (obj.x - point[0]) / dir[0];
    farX = (obj.x + obj.w - point[0]) / dir[0];
  } else {
    if (obj.x - point[0] > 0)
      nearX = DBL_MAX;
    else if (obj.x - point[0] < 0)
      nearX = -DBL_MAX;
    else
      nearX = 0;
    if (obj.x + obj.w - point[0] > 0)
      farX = DBL_MAX;
    else if (obj.x + obj.w - point[0] < 0)
      farX = -DBL_MAX;
    else
      farX = 0;
  }
  if (nearX > farX) swap(nearX, farX);
  if (nearY > farY) swap(nearY, farY);
  if (nearX > farY || nearY > farX) {
    return false;
  }
  nx = 0;
  ny = 0;
  if (nearX < nearY) {
    t = nearY;
    ny = -dir[1];
  } else {
    t = nearX;
    nx = -dir[0];
  }
  if (t >= 0 && t <= 1) {
    return true;
  }
  return false;
}

void Player::checkCollision(Player* p) {
  SDL_Rect p_loc = p->getLocation();
  SDL_Rect prev_loc = p->getPrevLocation();
  if (SDL_RectEquals(&location, &prev_location) &&
      SDL_RectEquals(&p_loc, &prev_loc)) {
    return;
  }
  SDL_Rect loc;
  if (collided)
    loc = location;
  else
    loc = prev_location;
  int x1 = loc.x, y1 = loc.y, x2 = prev_loc.x, y2 = prev_loc.y;
  int v_x = 0, v_y = 0, v1_x = 0, v2_x = 0, v1_y = 0, v2_y = 0;
  // if (x1 < x2 + p_loc.w && x1 + location.w > x2 && y1 < y2 + p_loc.h &&
  //     y1 + location.h > y2) {
  //   int t = 0;
  // }
  int v2 = p->getVelocity();
  v_x = location.x - x1;
  v_y = location.y - y1;
  v1_x = v_x;
  v1_y = v_y;
  v_x += (x2 - p_loc.x);
  v_y += (y2 - p_loc.y);
  v2_x = p_loc.x - x2;
  v2_y = p_loc.y - y2;

  vector<double> c1 = {(double)(2 * x1 + location.w) / 2,
                       (double)(2 * y1 + location.h) / 2};
  vector<double> dir = {(double)v_x, (double)v_y};
  SDL_FRect obj = {(double)(2 * x2 - location.w) / 2,
                   (double)(2 * y2 - location.h) / 2,
                   (double)location.w + p_loc.w, (double)location.h + p_loc.h};
  double t, nx, ny;
  vector<double> change1, change2;
  bool b = rayRectCollision(c1, dir, obj, t, nx, ny);
  if (!b) return;
  bool stopP1 = false, stopP2 = false;
  if (v1_x * v2_x > 0 || v1_y * v2_y > 0) {
    change1 = {(double)v1_x * t + (double)(v1_x + v2_x) * (1 - t) / 2,
               (double)v1_y * t + (double)(v1_y + v2_y) * (1 - t) / 2};
    change2 = {(double)v2_x * t + (double)(v1_x + v2_x) * (1 - t) / 2,
               (double)v2_y * t + (double)(v1_y + v2_y) * (1 - t) / 2};
    x1 += (int)change1[0];
    y1 += (int)change1[1];
    x2 += (int)change2[0];
    y2 += (int)change2[1];
    location.x = x1;
    location.y = y1;
    p->setLocation({x2, y2, p_loc.w, p_loc.h});
    stopP1 = true;
    stopP2 = true;
  } else {
    change1 = {(double)v1_x * t, (double)v1_y * t};
    change2 = {(double)v2_x * t, (double)v2_y * t};
    if (v1_x * nx < 0) {
      location.x = x1 + ((int)change1[0]);
      stopP1 = true;
    }
    if (v1_y * ny < 0) {
      location.y = y1 + ((int)change1[1]);
      stopP1 = true;
    }
    if (v2_x * nx > 0) {
      p_loc.x = x2 + ((int)change2[0]);
      stopP2 = true;
    }
    if (v2_y * ny > 0) {
      p_loc.y = y2 + ((int)change2[1]);
      stopP2 = true;
    }

    p->setLocation(p_loc);
  }
  if (stopP2) p->setPrevLocation(p->getLocation());
  if (stopP1) collided = true;
}
