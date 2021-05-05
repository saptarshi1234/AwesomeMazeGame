#pragma once
#include "player.hpp"

class Bot : public Player {
 private:
  int waitFrame = 0;
  int fireFreq = 5;

 public:
  void update(SDL_Rect loc, Maze* maze);
  void setFireFreq(int f);
  bool shouldFire();
};