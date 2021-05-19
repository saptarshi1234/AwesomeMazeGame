#pragma once
#include "player.hpp"

class Bot : public Player {
 private:
  int waitFire;
  int waitUpdate;
  int fireFreq;
  int updateFreq;
  Direction last_direction;

 public:
  Bot();
  void setHP(double h);
  void updateMove();
  void init(SDL_Rect loc);
  void update(SDL_Rect loc1, SDL_Rect loc2, Maze* maze);
  void setFireFreq(int f);
  void setUpdateFreq(int f);
  bool shouldFire();
  bool shouldUpdate();
  bool directionChanged();

  // std::string to_string();
  std::string to_update(int index);

  // void update_from_string(std::string s);
  // void create_from_string(std::string s);
};