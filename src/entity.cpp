#include "entity.hpp"

#include <iostream>
#include <sstream>

#include "params.hpp"

Entity::Entity() {}

Entity::~Entity() {
  // delete texture;
}

void Entity::init(SDL_Rect loc) {
  location = loc;
  velocity = 4;
  dir = RIGHT;
  is_moving = false;
}

void Entity::setSize(int w, int h) {
  location.h = h;
  location.w = w;
}

SDL_Rect Entity::getLocation() { return location; }
std::vector<LayerDetails> Entity::getLayers() { return layers; }

void Entity::setLocation(SDL_Rect rect) { location = rect; }
Direction Entity::getDirection() { return dir; }

bool notWall(Maze* maze, int x, int y) {
  auto m = maze->getPixelV();
  return m[y / Params::ACTUAL_CELL_SIZE][x / Params::ACTUAL_CELL_SIZE] == 0;
}

// SDL_Rect Entity::getCropArea() {
//   // SDL_Rect src;
//   // src.x = 0;
//   // src.y = 0;
//   // SDL_QueryTexture(texture, NULL, NULL, &src.w, &src.h);
//   // return src;
// }

void Entity::move(Maze* maze) {
  if (is_moving) {
    int tempX = location.x, tempY = location.y, x = location.x,
        fx = x + location.w, y = location.y, fy = y + location.h;
    switch (dir) {
      case TOP:
        tempY -= velocity;
        fy = y;
        y = tempY;
        break;
      case BOTTOM:
        tempY += velocity;
        y = fy;
        fy = y + velocity;
        break;
      case LEFT:
        tempX -= velocity;
        fx = x;
        x = tempX;
        break;
      case RIGHT:
        tempX += velocity;
        x = fx;
        fx = x + velocity;
        break;
      default:
        break;
    }
    if (tempX != location.x || tempY != location.y) {
      for (int i = x; i < fx; i++) {
        for (int j = y; j < fy; j++) {
          if (i < 0 || i >= Params::SCREEN_WIDTH || j < 0 ||
              j >= Params::SCREEN_HEIGHT || !notWall(maze, i, j)) {
            is_moving = false;
            break;
          }
        }
      }
      if (is_moving) {
        location.x = tempX;
        location.y = tempY;
        moves++;
      }
    }
  }
}

bool Entity::canMove(Maze* maze) {
  if (is_moving) {
    int tempX = location.x, tempY = location.y, x = location.x,
        fx = x + location.w, y = location.y, fy = y + location.h;
    switch (dir) {
      case TOP:
        tempY -= velocity;
        fy = y;
        y = tempY;
        break;
      case BOTTOM:
        tempY += velocity;
        y = fy;
        fy = y + velocity;
        break;
      case LEFT:
        tempX -= velocity;
        fx = x;
        x = tempX;
        break;
      case RIGHT:
        tempX += velocity;
        x = fx;
        fx = x + velocity;
        break;
      default:
        break;
    }
    if (tempX != location.x || tempY != location.y) {
      for (int i = x; i < fx; i++) {
        for (int j = y; j < fy; j++) {
          if (i < 0 || i >= Params::SCREEN_WIDTH || j < 0 ||
              j >= Params::SCREEN_HEIGHT || !notWall(maze, i, j)) {
            return false;
          }
        }
      }
      return true;
    }
    return false;
  }
  return false;
}

void Entity::stopMoving() { is_moving = false; }
bool Entity::isMoving() { return is_moving; }

void Entity::setDirection(Direction d) {
  dir = d;
  is_moving = true;
}

std::string Entity::to_string() {
  std::stringstream ss;
  char space = ' ';
  ss << location.x << space << location.y << space << location.h << space
     << location.w << space << is_moving << space << dir;
  return ss.str();
}

void Entity::from_string(std::string s) {
  int int_dir;
  std::stringstream ss(s);
  ss >> location.x;
  ss >> location.y;
  ss >> location.h;
  ss >> location.w;
  ss >> is_moving;
  ss >> int_dir;

  dir = Direction(int_dir);
}