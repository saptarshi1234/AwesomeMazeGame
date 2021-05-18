#include "player.hpp"

#include <iostream>

#include "bullet.hpp"
#include "items.hpp"
#include "textures.hpp"

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
void Player::raiseScore(int s) { score += s * score_multiplier; }
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
  // if (moves % 2 == 0) {
  //   layers[0].tex = F1_tex;
  // } else {
  //   layers[0].tex = F2_tex;
  // }

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
