#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <vector>

enum Direction { TOP, LEFT, BOTTOM, RIGHT };

class Entity {
 private:
  SDL_Texture* texture;
  SDL_Rect location;
  int offsetX;
  int offsetY;
  Direction dir = RIGHT;

  int velocity;

 public:
  Entity();
  void init(SDL_Rect loc, SDL_Texture* tex);
  void setSize(int, int);
  SDL_Rect getLocation();
  SDL_Texture* getTexture();
  Direction getDirection();

  void move(Direction toMove);

  void setLocation(SDL_Rect rect);
};