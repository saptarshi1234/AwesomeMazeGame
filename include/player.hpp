#pragma once

#include <map>
#include <vector>

#include "bullet.hpp"
#include "entity.hpp"
#include "items.hpp"
class Player : public Entity {
 protected:
  SDL_Texture *F1_tex;
  SDL_Texture *F2_tex;
  // SDL_Texture *F1_inv;
  // SDL_Texture *F2_tex;
  SDL_Texture *gun;
  SDL_Texture *explosion;
  bool is_bot = false;
  int firing = 0;
  int score = 0;
  int score_multiplier = 1;
  double bullet_power = 1;
  double hp = Params::MAX_HP;
  std::vector<int> collected;
  bool collided;

 public:
  // Player();

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
  void updateItems();
  void collectItem(Item &item);
  std::vector<int> getCollectedItems();
  void move(Maze *maze);
  void updateState();
  void checkCollision(Player *p);
  SDL_Rect getCropArea();

  std::string to_string();
  std::string to_update();

  void update_from_string(std::string s);
  void create_from_string(std::string s);
};
