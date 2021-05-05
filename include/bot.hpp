#pragma once
#include "player.hpp"

class Bot : public Player {
 private:
  int waitFire = 0;
  int waitUpdate = 0;
  int fireFreq = 20;
  int updateFreq = 50;

 public:
  void update(SDL_Rect loc, Maze* maze);
  void setFireFreq(int f);
  void setUpdateFreq(int f);
  bool shouldFire();
  bool shouldUpdate();
};