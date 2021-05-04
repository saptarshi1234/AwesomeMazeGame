#include "entity.hpp"

#include <iostream>

#include "params.hpp"
Entity::Entity() {}

Entity::~Entity() {
  // delete texture;
}

void Entity::init(SDL_Rect loc, SDL_Texture* tex) {
  location = loc;
  texture = tex;
  velocity = 2;
  offsetX = 0;
  offsetY = 0;
  dir = RIGHT;
  is_moving = false;
}

void Entity::setSize(int w, int h) {
  location.h = h;
  location.w = w;
}

SDL_Rect Entity::getLocation() { return location; }
SDL_Texture* Entity::getTexture() { return texture; }

void Entity::setLocation(SDL_Rect rect) { location = rect; }
Direction Entity::getDirection() { return dir; }

bool notWall(Maze* maze, int x, int y) {
  auto m = maze->getPixelV();
  return m[y / Params::ACTUAL_CELL_SIZE][x / Params::ACTUAL_CELL_SIZE] == 0;
}

void Entity::move(Maze* maze) {
  if (is_moving) {
    int tempX = location.x, tempY = location.y;
    switch (dir) {
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
      default:
        break;
    }
    if (tempX != location.x || tempY != location.y) {
      int x = location.x, fx = x + location.w, y = location.y,
          fy = y + location.h;
      if (tempX < x) {
        fx = x;
        x = tempX;
      } else if (tempX > x) {
        x = fx;
        fx = x + velocity;
      } else if (tempY < y) {
        fy = y;
        y = tempY;
      } else {
        y = fy;
        fy = y + velocity;
      }
      for (int i = x; i < fx; i++) {
        for (int j = y; j < fy; j++) {
          if (i >= Params::SCREEN_WIDTH || j >= Params::SCREEN_HEIGHT ||
              !notWall(maze, i, j)) {
            is_moving = false;
            break;
          }
        }
      }
      if (is_moving) {
        location.x = tempX;
        location.y = tempY;
      }
    }
  }
}

void Entity::stopMoving() { is_moving = false; }
bool Entity::isMoving() { return is_moving; }

void Entity::setDirection(Direction d) {
  dir = d;
  is_moving = true;
}
