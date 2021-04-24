#include "entity.hpp"

#include <iostream>

#include "params.hpp"
Entity::Entity() {}

void Entity::init(SDL_Rect loc, SDL_Texture* tex) {
  location = loc;
  texture = tex;
  velocity = 10;
  offsetX = 0;
  offsetY = 0;
}

void Entity::setSize(int w, int h) {
  location.h = h;
  location.w = w;
}

SDL_Rect Entity::getLocation() { return location; }
SDL_Texture* Entity::getTexture() { return texture; }

void Entity::setLocation(SDL_Rect rect) { location = rect; }
Direction Entity::getDirection() { return dir; }

void Entity::move(Direction toMove) {
  dir = toMove;

  std::cout << location.x << ". " << location.y << std::endl;

  switch (toMove) {
    case TOP:
      if (location.y - velocity >= 0) location.y -= velocity;
      break;
    case BOTTOM:
      if (location.y + location.h + velocity <= Params::SCREEN_HEIGHT)
        location.y += velocity;
      break;
    case LEFT:
      if (location.x - velocity >= 0) location.x -= velocity;
      break;
    case RIGHT:
      if (location.x + location.w + velocity <= Params::SCREEN_WIDTH)
        location.x += velocity;
      break;
    default:
      break;
  }
}