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
  velocity = 8;
  dir = RIGHT;
  is_moving = false;
  prev_location = loc;
}

void Entity::setSize(int w, int h) {
  location.h = h;
  location.w = w;
}

void Entity::setLayers(std::vector<LayerDetails> layers) {
  this->layers = layers;
}

SDL_Rect Entity::getLocation() { return location; }

SDL_Rect Entity::getPhysicalLocation() { return {}; }

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
  prev_location = location;
  int factor = 4;
  if (is_moving) {
    int tempX = location.x, tempY = location.y, x = location.x,
        fx = x + location.w, y = location.y, fy = y + location.h;
    bool x_free = false, y_free = false;
    int k = 1;
    switch (dir) {
      case TOP:
        tempY -= velocity;
        fy = tempY - 1;
        y -= 1;
        k = -1;
        x_free = true;
        break;
      case RIGHT:
        tempX += velocity;
        x = fx;
        fx = x + velocity;
        y_free = true;
        break;
      case BOTTOM:
        tempY += velocity;
        y = fy;
        fy = y + velocity;
        x_free = true;
        break;
      case LEFT:
        x -= 1;
        tempX -= velocity;
        fx = tempX - 1;
        k = -1;
        y_free = true;
        break;

      default:
        break;
    }
    int min_ = -1, max_ = -1;
    int limit_x = 0, limit_y = 0;
    if (tempX != location.x || tempY != location.y) {
      if (y_free) {
        int i = x;
        while (i != fx) {
          for (int j = y; j < fy; j++) {
            if (i < 0 || i >= Params::SCREEN_WIDTH || j < 0 ||
                j >= Params::SCREEN_HEIGHT || !notWall(maze, i, j)) {
              if (j >= y + location.h / factor &&
                  j < fy - location.h / factor) {
                is_moving = false;
                break;
              }
            } else {
              if (i == fx - k) {
                if (min_ == -1) min_ = j;
                max_ = j;
              }
            }
          }
          if (!is_moving) break;
          i += k;
          limit_x += k;
        }
      } else {
        int j = y;
        while (j != fy) {
          for (int i = x; i < fx; i++) {
            if (i < 0 || i >= Params::SCREEN_WIDTH || j < 0 ||
                j >= Params::SCREEN_HEIGHT || !notWall(maze, i, j)) {
              if (i >= x + location.w / factor &&
                  i < fx - location.w / factor) {
                is_moving = false;
                break;
              }
            } else {
              if (j == fy - k) {
                if (min_ == -1) min_ = i;
                max_ = i;
              }
            }
          }
          if (!is_moving) break;
          j += k;
          limit_y += k;
        }
      }
      if (is_moving) {
        if (x_free) {
          if (tempX != min_)
            tempX = min_;
          else if (tempX + location.w - 1 != max_)
            tempX = max_ + 1 - location.w;
        } else {
          if (tempY != min_)
            tempY = min_;
          else if (tempY + location.h - 1 != max_)
            tempY = max_ + 1 - location.h;
        }
        location.x = tempX;
        location.y = tempY;
      } else {
        location.x += limit_x;
        location.y += limit_y;
      }
      if (limit_x != 0 || limit_y != 0) moves++;
    }
  }
}

bool Entity::canMove(Maze* maze) {
  int factor = 4;

  if (is_moving) {
    int x = location.x, fx = x + location.w, y = location.y,
        fy = y + location.h;
    bool x_free = true;
    switch (dir) {
      case TOP:
        fy = y;
        y = fy - 3;
        fx -= location.w / factor;
        x += location.w / factor;
        break;
      case BOTTOM:
        y = fy;
        fy = y + 3;
        fx -= location.w / factor;
        x += location.w / factor;
        break;
      case LEFT:
        fx = x;
        x = fx - 3;
        fy -= location.h / factor;
        y += location.h / factor;
        x_free = false;
        break;
      case RIGHT:
        x = fx;
        fx = x + 3;
        fy -= location.h / factor;
        y += location.h / factor;
        x_free = false;
        break;
      default:
        break;
    }
    if (x != location.x || y != location.y) {
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

SDL_Rect Entity::getPrevLocation() { return prev_location; }
void Entity::setPrevLocation(SDL_Rect loc) { prev_location = loc; }

int Entity::getVelocity() { return velocity; }
void Entity::setVelocity(int v) { velocity = v; }
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
