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
  dir = STOP;
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
    switch (dir) {
      case TOP:
        if (notWall(maze, location.x, location.y - velocity))
          location.y -= velocity;
        else {
          is_moving = false;
        }
        break;
      case BOTTOM:
        if (notWall(maze, location.x, location.y + location.h + velocity - 1))
          location.y += velocity;
        else {
          is_moving = false;
        }
        break;
      case LEFT:
        if (notWall(maze, location.x - velocity, location.y))
          location.x -= velocity;
        else {
          is_moving = false;
        }
        break;
      case RIGHT:
        if (notWall(maze, location.x + location.w + velocity - 1, location.y))
          location.x += velocity;
        else {
          is_moving = false;
        }
        break;
      default:
        break;
    }
  }
}

void Entity::stopMoving() { is_moving = false; }
bool Entity::isMoving() { return is_moving; }

void Entity::setDirection(Direction d) {
  dir = d;
  is_moving = true;
}
