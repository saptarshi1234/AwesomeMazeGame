#pragma once
#include "player.hpp"

class Bot : public Player {
 private:
  int waitFire;
  int waitUpdate;
  int fireFreq;
  int updateFreq;

 public:
  Bot();
  void update(SDL_Rect loc, Maze* maze);
  void setFireFreq(int f);
  void setUpdateFreq(int f);
  bool shouldFire();
  bool shouldUpdate();
};