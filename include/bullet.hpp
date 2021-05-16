#pragma once
#include "entity.hpp"
#include "params.hpp"
class Player;
class Bullet : public Entity {
 private:
  bool firedByPlayer = false;
  Player *shooter;
  double power = Params::BULLET_POWER_UNIT;
  bool destroy = false;

 public:
  void init(SDL_Rect loc, bool fired_by, Player *player, double p);
  void setDirection(Direction dir);
  bool destroyBullet();
  bool hitTarget(Player &p);
  // void move(Maze *m);
};