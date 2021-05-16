#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <string>
#include <vector>

#include "maze.hpp"

enum Direction { TOP, LEFT, BOTTOM, RIGHT, STOP };

struct LayerDetails {
  SDL_Texture* tex;
  std::pair<int, int> offset = {0, 0};
  std::pair<int, int> crop_details = {-1, -1};
  bool toShow = true;
  std::pair<int, int> dstSize = {-1, -1};

  SDL_Rect getSize() {
    SDL_Rect src;
    src.x = 0;
    src.y = 0;
    SDL_QueryTexture(tex, NULL, NULL, &src.w, &src.h);
    return src;
  }
};

class Entity {
 protected:
  std::vector<LayerDetails> layers;
  SDL_Rect location;
  int velocity;

  Direction dir;
  bool is_moving;

 public:
  Entity();
  ~Entity();
  int moves = 0;

  void init(SDL_Rect loc);
  void setSize(int, int);
  SDL_Rect getLocation();
  virtual SDL_Rect getPhysicalLocation();

  std::vector<LayerDetails> getLayers();
  void setLayers(std::vector<LayerDetails> layers);
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