#include "player.hpp"

#include <iostream>

#include "bullet.hpp"
#include "textures.hpp"

void Player::init(SDL_Rect loc) {
  int w = 2 * Params::ACTUAL_CELL_SIZE * 8 / 10;
  int h = 2 * Params::ACTUAL_CELL_SIZE / 10;

  int offX = Params::ACTUAL_CELL_SIZE / 2;

  Entity::init(loc);
  F1_tex = TextureManager::getTex(TextureID::TANK_F1);
  F2_tex = TextureManager::getTex(TextureID::TANK_F2);
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
  int recoil = 5, shift = 5;
  switch (dir) {
    case TOP:
      layers[1].offset = {0, recoil};
      break;
    case RIGHT:
      layers[1].offset = {-recoil, 0};
      break;
    case BOTTOM:
      layers[1].offset = {0, -recoil};
      break;
    case LEFT:
      layers[1].offset = {recoil, 0};
      break;
    default:
      break;
  }
  return b;
}

void Player::setHP(double h) {
  hp = h;
  layers[4].dstSize.first =
      (int)(Params::ACTUAL_CELL_SIZE * 16 / 10 * hp / Params::MAX_HP);
}
void Player::setBulletPower(double x) { bullet_power = x; }
void Player::raiseScore(int s) { score += s; }
double Player::getHP() { return hp; }
double Player::getBulletPower() { return bullet_power; }
int Player::getScore() { return score; }
bool Player::isBot() { return is_bot; }

void Player::move(Maze* maze) {
  Entity::move(maze);
  if (firing > 0) firing++;
  if (firing == 4) {
    layers[1].offset = {0, 0};
    firing = 0;
  }
  if (moves % 2 == 0) {
    layers[0].tex = F1_tex;
  } else {
    layers[0].tex = F2_tex;
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

void Player::takeItem(Item& item) {
  ;
  ;
}