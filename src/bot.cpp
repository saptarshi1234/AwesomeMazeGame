#include "bot.hpp"

#include <iostream>
#include <random>

#include "maze.hpp"
#include "params.hpp"

int convDir(Direction d) {
  switch (d) {
    case TOP:
      return 0;
      break;
    case LEFT:
      return 1;
      break;
    case BOTTOM:
      return 2;
      break;
    case RIGHT:
      return 3;
      break;
    default:
      break;
  }
}

Direction convtInt(int i) {
  switch (i) {
    case 0:
      return TOP;
      break;
    case 1:
      return LEFT;
      break;
    case 2:
      return BOTTOM;
      break;
    case 3:
      return RIGHT;
      break;
    default:
      break;
  }
}

Bot::Bot() {
  waitFire = 0;
  waitUpdate = 0;
  updateFreq = 50;
  fireFreq = rand() % 20 + 20;
}

void Bot::init(SDL_Rect loc) {
  Player::init(loc);
  is_bot = true;
  hp = 100;
}

void Bot::setHP(double h) {
  hp = h;
  layers[4].dstSize.first =
      (int)(Params::ACTUAL_CELL_SIZE * 16 / 10 * hp / Params::MAX_BOT_HP);
}

void Bot::update(SDL_Rect loc, Maze* maze) {
  int probs[] = {2, 2, 2, 2};
  Direction dir = Entity::getDirection();
  last_direction = dir;

  probs[convDir(dir)] += 8;
  int curD = abs(location.x - loc.x) + abs(location.y - loc.y);
  int tot = 0;
  for (int i = 0; i < 4; i++) {
    Direction dire = convtInt(i);
    Entity::setDirection(dire);
    if (!(Entity::canMove(maze))) {
      probs[i] = 0;
      continue;
    } else {
      int tempX = location.x, tempY = location.y;
      switch (dire) {
        case TOP:
          tempY -= velocity;
          break;
        case BOTTOM:
          tempY += velocity;
          break;
        case LEFT:
          tempX -= velocity;
          break;
        case RIGHT:
          tempX += velocity;
          break;
      }
      int D = abs(tempX - loc.x) + abs(tempY - loc.y);
      if (D < curD) probs[i] += 20;
      tot += probs[i];
    }
  }
  int k = rand() % tot;
  int lef = 0;
  for (int i = 0; i < 4; i++) {
    if (k >= lef && k < probs[i] + lef) {
      Entity::setDirection(convtInt(i));
      break;
    }
    lef += probs[i];
  }
}

void Bot::setFireFreq(int f) { fireFreq = f; }

bool Bot::shouldFire() {
  waitFire = (waitFire + 1) % fireFreq;
  return (waitFire == 0);
}

void Bot::setUpdateFreq(int f) { updateFreq = f; }

bool Bot::shouldUpdate() {
  waitUpdate = (waitUpdate + 1) % updateFreq;
  return (waitUpdate == 0);
}

std::string Bot::to_update(int index) {
  std::stringstream ss;
  char space = ' ';
  ss << index << space << dir;

  return ss.str();
}

bool Bot::directionChanged() { return last_direction != dir; }