#pragma once
#include "entity.hpp"

class Player;

class Item : public Entity {
 public:
  enum ItemType { MULTIPLIER = 0, INVISIBLE, POWERUP, SHIELD };
  const static int numTypes = 4;
  int timeLeft = 10;

  Item();
  ItemType getType();
  void init(SDL_Rect loc, ItemType type);
  bool checkCollected(Player &p);
  SDL_Rect getLocation();
  virtual SDL_Rect getPhysicalLocation();

 private:
  ItemType type;
};