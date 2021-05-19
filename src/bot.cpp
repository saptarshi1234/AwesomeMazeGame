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
      return -1;
      break;
  }
}

Direction convInt(int i) {
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
      return STOP;
      break;
  }
}

int dX(Direction d, int v) {
  switch (d) {
    case LEFT:
      return -v;
      break;
    case RIGHT:
      return v;
      break;
    default:
      return 0;
      break;
  }
}

int dY(Direction d, int v) {
  switch (d) {
    case TOP:
      return -v;
      break;
    case BOTTOM:
      return v;
      break;
    default:
      return 0;
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
  is_bot = true;
  Player::init(loc);
  hp = 100;
}

void Bot::setHP(double h) {
  hp = h;
  layers[4].dstSize.first =
      (int)(Params::ACTUAL_CELL_SIZE * 16 / 10 * hp / Params::MAX_BOT_HP);
}

void Bot::update(SDL_Rect loc1, SDL_Rect loc2, bool b1, bool b2, Maze* maze) {
  int dist1 = maze->dist(location.x, location.y, loc1.x, loc1.y);
  int dist2 = maze->dist(location.x, location.y, loc2.x, loc2.y);
  if (dist1 == -1 || b1) dist1 = INT32_MAX;
  if (dist2 == -1 || b2) dist2 = INT32_MAX;
  SDL_Rect loc;
  int dist;
  if (dist1 - 3 > dist2) {
    loc = loc2;
    chosen = -1;
  } else if (dist2 - 3 > dist1) {
    chosen = -1;
    loc = loc1;
  } else {
    if (chosen == 1) {
      loc = loc1;
    } else if (chosen == 2) {
      loc = loc2;
    } else {
      int t = rand() % 2;
      if (t == 0) {
        loc = loc1;
        chosen = 1;
      } else {
        loc = loc2;
        chosen = 2;
      }
    }
  }
  bool moving = is_moving;
  dist = maze->dist(location.x, location.y, loc.x, loc.y);
  int movable[] = {0, 0, 0, 0};
  int prob[] = {0, 0, 0, 0};
  int count = 0;
  Direction curr_dir = Entity::getDirection();
  if (dist1 != INT32_MAX || dist2 != INT32_MAX) {
    Direction p_dir = convInt(maze->dirFromTo(location.x, location.y, loc.x,
                                              loc.y, convDir(curr_dir)));
    Direction p_dirF = p_dir;
    Entity::setDirection(p_dir);
    bool found = canMove(maze);
    if (found) {
      count++;
      movable[convDir(p_dir)] = 1;
      prob[convDir(p_dir)] = 90;
    }
    if (curr_dir != p_dir) {
      Entity::setDirection(curr_dir);
      if (canMove(maze)) {
        count++;
        movable[convDir(curr_dir)] = 1;
        prob[convDir(curr_dir)] = 2;
        if (!found && maze->dist(location.x + dX(curr_dir, velocity),
                                 location.y + dY(curr_dir, velocity), loc.x,
                                 loc.y) <= dist) {
          found = true;
          p_dirF = curr_dir;
          prob[convDir(p_dirF)] = 90;
        }
      }
    }

    for (int i = 0; i < 4; i++) {
      if (i == convDir(p_dir) || i == convDir(curr_dir)) continue;
      Direction d = convInt(i);
      Entity::setDirection(d);
      if (canMove(maze)) {
        movable[i] = 1;
        prob[i] = 2;
        count++;
        if (!found && i + convDir(curr_dir) % 2 != 0 &&
            maze->dist(location.x + dX(d, velocity),
                       location.y + dY(d, velocity), loc.x, loc.y) <= dist) {
          found = true;
          p_dirF = d;
          prob[convDir(p_dirF)] = 90;
        }
      }
    }

    int opp = convDir(curr_dir) - 2;
    if (opp < 0) opp = convDir(curr_dir) + 2;
    prob[opp] = 0;
    if (!found) {
      Direction d = convInt(opp);
      if (maze->dist(location.x + dX(d, velocity), location.y + dY(d, velocity),
                     loc.x, loc.y) <= dist) {
        found = true;
        p_dirF = d;
        if (prob[convDir(curr_dir)] == 0) {
          if (dist < 15) prob[opp] = 90;
        }
      }
    }
    if (dist < 10 || count == 4) {
      if (maze->inLine(location.x, location.y, loc.x, loc.y, convDir(p_dirF))) {
        Entity::setDirection(p_dirF);
        int o = rand() % 10;
        if (!moving || dist == 1 || o >= dist) {
          is_moving = false;
        }
        return;
      }
      Entity::setDirection(p_dirF);
      return;
    }

    if (dist >= 15) {
      if (prob[convDir(curr_dir)] > 0 && curr_dir != p_dirF &&
          convDir(p_dirF) != opp) {
        prob[p_dirF] = 20;
      }
      for (int i = 0; i < 4; i++) {
        if (prob[i] > 0 && prob[i] < 90) {
          if (convDir(curr_dir) == i) {
            prob[convDir(curr_dir)] = 90;
          } else if (i == opp) {
            if (prob[convDir(curr_dir)] == 0) {
              prob[i] = 20;
            }
          } else
            prob[i] = 20;
        }
      }
    }
  } else {
    int count = 0;
    int opp = convDir(curr_dir) - 2;
    if (opp < 0) opp = convDir(curr_dir) + 2;
    for (int i = 0; i < 4; i++) {
      Entity::setDirection(convInt(i));
      if (canMove(maze)) {
        movable[i] = 1;
        count++;
      }
    }
    for (int i = 0; i < 4; i++) {
      if (movable[i] == 1) {
        if (convInt(i) == curr_dir) {
          prob[i] = 90;
        } else if (i == opp) {
          if (count == 1) {
            prob[i] == 0;
          }
        } else {
          prob[i] = 20;
        }
      }
    }
  }
  int tot = 0;
  for (int i = 0; i < 4; i++) {
    tot += prob[i];
  }
  if (tot == 0) {
    return;
  }
  int k = rand() % tot;
  int lef = 0;
  for (int i = 0; i < 4; i++) {
    if (k >= lef && k < prob[i] + lef) {
      Entity::setDirection(convInt(i));
      break;
    }
    lef += prob[i];
  }
}

// int probs[] = {2, 2, 2, 2};
// Direction dir = Entity::getDirection();
// probs[convDir(dir)] += 8;
// int curD = abs(location.x - loc.x) + abs(location.y - loc.y);
// int tot = 0;
// for (int i = 0; i < 4; i++) {
//   Direction dire = convtInt(i);
//   Entity::setDirection(dire);
//   if (!(Entity::canMove(maze))) {
//     probs[i] = 0;
//     continue;
//   } else {
//     int tempX = location.x, tempY = location.y;
//     switch (dire) {
//       case TOP:
//         tempY -= velocity;
//         break;
//       case BOTTOM:
//         tempY += velocity;
//         break;
//       case LEFT:
//         tempX -= velocity;
//         break;
//       case RIGHT:
//         tempX += velocity;
//         break;
//     }
//     int D = abs(tempX - loc.x) + abs(tempY - loc.y);
//     if (D < curD) probs[i] += 20;
//     tot += probs[i];
//   }
// }
// int k = rand() % tot;
// int lef = 0;
// for (int i = 0; i < 4; i++) {
//   if (k >= lef && k < probs[i] + lef) {
//     Entity::setDirection(convtInt(i));
//     break;
//   }
//   lef += probs[i];
// }

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
// invisibility case remain

void Bot::updateMove() {
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