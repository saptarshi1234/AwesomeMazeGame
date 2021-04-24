#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <vector>

enum Direction { TOP, LEFT, BOTTOM, RIGHT, STOP };

class Entity {
 protected:
  SDL_Texture* texture;
  SDL_Rect location;
  int offsetX;
  int offsetY;
  int velocity;

  Direction dir;

 public:
  Entity();
  ~Entity();

  void init(SDL_Rect loc, SDL_Texture* tex);
  void setSize(int, int);
  SDL_Rect getLocation();
  SDL_Texture* getTexture();
  Direction getDirection();

  void move();
  void setDirection(Direction d);
  void setLocation(SDL_Rect rect);
};