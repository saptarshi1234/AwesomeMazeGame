#pragma once

#include "bullet.hpp"
#include "entity.hpp"

class Item;
class Player : public Entity {
 protected:
  SDL_Texture *F1_tex;
  SDL_Texture *F2_tex;
  SDL_Texture *gun;
  SDL_Texture *explosion;
  bool is_bot = false;
  int firing = 0;
  int score = 0;
  double bullet_power = 1;
  double hp = Params::MAX_HP;

 public:
  int explosion_status = 0;
  void init(SDL_Rect loc);
  Bullet fireBullet();
  SDL_Rect getPhysicalLocation();
  void setHP(double h);
  void setBulletPower(double x);
  void raiseScore(int s);
  double getHP();
  double getBulletPower();
  int getScore();
  bool isBot();
  void takeItem(Item &item);
  void move(Maze *maze);
  SDL_Rect getCropArea();
};