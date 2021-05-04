#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stack>
#include <vector>

struct cell {
  bool visited = false;
  bool down = false;   // if conected to down
  bool right = false;  // if connected to right
  int prvsX;
  int prvsY;
  int X;
  int Y;
};

class Maze {
 protected:
  int mazeWidth;
  int mazeHeight;
  cell** maze;
  int pathLength;
  int wallLength;
  std::vector<std::vector<int>> pixelV;

 public:
  Maze();
  ~Maze();
  SDL_Texture* maze_tex;

  void setSize(int w, int h);
  void setPathLength(int l);
  void setWallLength(int l);
  void generate();
  void draw();
  void addPadding();
  std::vector<std::vector<int>> render();
  std::vector<std::vector<int>> getPixelV();
  void setPixelV(std::vector<std::vector<int>> v);
};
