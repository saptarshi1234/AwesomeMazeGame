#pragma once
#include "entity.hpp"
#include "player.hpp"

class Item : public Entity {
 public:
  enum ItemType { MULTIPLIER = 0, INVISIBLE, POWERUP };

  Item();
  ItemType getType();
  void init(SDL_Rect loc, ItemType type);
  bool checkCollected(Player &p);
  SDL_Rect getLocation();
  virtual SDL_Rect getPhysicalLocation();

 private:
  ItemType type;
};