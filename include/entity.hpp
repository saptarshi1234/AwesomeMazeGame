#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <string>
#include <vector>

#include "maze.hpp"

enum Direction { TOP, LEFT, BOTTOM, RIGHT, STOP };

class Entity {
 protected:
  SDL_Texture* texture;
  SDL_Rect location;
  int offsetX;
  int offsetY;
  int velocity;
  int moves = 0;

  Direction dir;
  bool is_moving;

 public:
  Entity();
  ~Entity();

  void init(SDL_Rect loc, SDL_Texture* tex);
  void setSize(int, int);
  SDL_Rect getLocation();
  virtual SDL_Rect getCropArea();
  SDL_Texture* getTexture();
  Direction getDirection();

  void move(Maze* maze);
  bool canMove(Maze* maze);
  void stopMoving();
  bool isMoving();
  void setDirection(Direction d);
  void setLocation(SDL_Rect rect);
  std::string to_string();
  void from_string(std::string s);
};