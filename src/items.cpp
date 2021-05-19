#include "items.hpp"

#include <random>

#include "params.hpp"
#include "player.hpp"
#include "textures.hpp"

Item::Item() {}

void Item::init(SDL_Rect loc, ItemType type) {
  Entity::init(loc);
  itemDestroyCounter = 80 + (rand() % 20);
  this->type = type;

  SDL_Rect rect = getPhysicalLocation();

  std::map<ItemType, LayerDetails> layer_choices;

  layer_choices[MULTIPLIER] = {
      TextureManager::getTex(TextureID::MULTIPLIER_2X), {0, 0}, {0, 1}};
  layer_choices[INVISIBLE] = {
      TextureManager::getTex(TextureID::INVISIBLE), {0, 0}, {0, 1}};
  layer_choices[POWERUP] = {TextureManager::getTex(TextureID::POWERUP),
                            {0, 0},
                            {0, 8},
                            true,
                            {rect.w, rect.h}};
  layer_choices[SHIELD] = {
      TextureManager::getTex(TextureID::SHIELD), {0, 0}, {0, 1}};

  layer_choices[POWERUP].vertical_crop = false;

  layers = {layer_choices[type]};
}

Item::ItemType Item::getType() { return type; }
SDL_Rect Item::getPhysicalLocation() { return location; }

bool Item::checkCollected(Player &p) {
  auto p_loc = p.getLocation();

  if (location.x < p_loc.x + p_loc.w && location.x + location.w > p_loc.x &&
      location.y < p_loc.y + p_loc.h && location.y + location.h > p_loc.y) {
    return true;
  }
  return false;
}

std::string Item::to_string() {
  std::stringstream ss;
  char space = ' ';
  ss << location.x << space << location.y << space << location.w << space
     << location.h << space << type;
  return ss.str();
}

void Item::from_string(std::string s) {
  std::stringstream ss(s);
  int type_int;

  ss >> location.x;
  ss >> location.y;
  ss >> location.w;
  ss >> location.h;
  ss >> type_int;

  init(location, ItemType(type_int));
}

bool Item::shouldDestroy() {
  itemDestroyCounter--;
  return itemDestroyCounter <= 0;
}