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

void Entity::move() {
  if (is_moving) {
    switch (dir) {
      case TOP:
        if (location.y - velocity >= 0)
          location.y -= velocity;
        else {
          is_moving = false;
        }
        break;
      case BOTTOM:
        if (location.y + location.h + velocity <= Params::SCREEN_HEIGHT)
          location.y += velocity;
        else {
          is_moving = false;
        }
        break;
      case LEFT:
        if (location.x - velocity >= 0)
          location.x -= velocity;
        else {
          is_moving = false;
        }
        break;
      case RIGHT:
        if (location.x + location.w + velocity <= Params::SCREEN_WIDTH)
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
