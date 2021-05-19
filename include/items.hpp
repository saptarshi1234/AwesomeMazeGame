#pragma once
#include "entity.hpp"

class Player;

class Item : public Entity {
 public:
  enum ItemType { MULTIPLIER = 0, INVISIBLE, POWERUP, SHIELD, HP };
  const static int numTypes = 5;
  int timeLeft = 10;

  Item();
  ItemType getType();
  void init(SDL_Rect loc, ItemType type);
  bool checkCollected(Player &p);
  SDL_Rect getLocation();
  virtual SDL_Rect getPhysicalLocation();
  bool shouldDestroy();

  std::string to_string();
  void from_string(std::string s);

 private:
  ItemType type;
  int itemDestroyCounter;
};