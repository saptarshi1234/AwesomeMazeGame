#pragma once
#include <iostream>
#include <sstream>

#include "entity.hpp"
#include "params.hpp"
class Player;
class Bullet : public Entity {
 private:
  bool firedByPlayer = false;
  Player *shooter;
  SDL_Rect physicalLocation;
  double power = Params::BULLET_POWER_UNIT;
  bool destroy = false;

 public:
  void init(SDL_Rect loc, bool fired_by, Player *player, double p);
  SDL_Rect getPhysicalLocation();
  void setDirection(Direction dir);
  bool destroyBullet();
  bool hitTarget(Player &p);

  std::string to_string(Player *p1, Player *p2);
  void create_from_string(std::string s, Player *p1, Player *p2);
  // void move(Maze *m);
};