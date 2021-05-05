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

void Bot::update(SDL_Rect loc, Maze* maze) {
  waitFrame = (waitFrame + 1) % fireFreq;
  int probs[] = {1, 1, 1, 1};
  Direction dir = Entity::getDirection();
  probs[convDir(dir)] += 4;
  int curD = maze->dist(location.x / Params::ACTUAL_CELL_SIZE,
                        location.y / Params::ACTUAL_CELL_SIZE,
                        loc.x / Params::ACTUAL_CELL_SIZE,
                        loc.y / Params::ACTUAL_CELL_SIZE);
  int tot;
  for (int i = 0; i < 4; i++) {
    Direction dire = convtInt(i);
    Entity::setDirection(dire);
    if (!(Entity::canMove(maze))) {
      probs[i] = 0;
      continue;
    }
    int tempX = location.x, tempY = location.y;
    switch (dire) {
      case TOP:
        tempY -= velocity;
      case BOTTOM:
        tempY += velocity;
      case LEFT:
        tempX -= velocity;
      case RIGHT:
        tempX += velocity;
    }
    int D = maze->dist(
        tempX / Params::ACTUAL_CELL_SIZE, tempY / Params::ACTUAL_CELL_SIZE,
        loc.x / Params::ACTUAL_CELL_SIZE, loc.y / Params::ACTUAL_CELL_SIZE);
    if (D < curD) probs[i] += 3;
    tot += probs[i];
  }
  int k = rand() % tot;
  int lef = 0;
  for (int i = 0; i < 4; i++) {
    if (k >= lef; k < probs[i] + lef) {
      Entity::setDirection(convtInt(i));
      break;
    }
    lef += probs[i];
  }
}

void Bot::setFireFreq(int f) { fireFreq = f; }

bool Bot::shouldFire() { return (waitFrame == 0); }
