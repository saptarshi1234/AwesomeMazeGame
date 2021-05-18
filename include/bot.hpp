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
  void init(SDL_Rect loc);
  void update(SDL_Rect loc1, SDL_Rect loc2, Maze* maze);
  void setFireFreq(int f);
  void setUpdateFreq(int f);
  bool shouldFire();
  bool shouldUpdate();
};