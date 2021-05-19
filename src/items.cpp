#include "items.hpp"

#include "params.hpp"
#include "player.hpp"
#include "textures.hpp"

Item::Item() {}

void Item::init(SDL_Rect loc, ItemType type) {
  Entity::init(loc);
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
                            {rect.w, rect.h * 2}};
  layer_choices[SHIELD] = {
      TextureManager::getTex(TextureID::SHIELD), {0, 0}, {0, 1}};

  layer_choices[POWERUP].vertical_crop = false;

  layers = {layer_choices[type]};
}

Item::ItemType Item::getType() { return type; }
SDL_Rect Item::getPhysicalLocation() { return location; }

bool Item::checkCollected(Player &p) {
  auto cell1 = p.getLocation();
  auto cell2 = p.getLocation();

  int w = Params::ACTUAL_CELL_SIZE * Params::PATH_WIDTH;

  switch (p.getDirection()) {
    case TOP:
      cell2.y -= w;
      break;
    case RIGHT:
      cell2.x += w;
      break;
    case BOTTOM:
      cell2.y += w;
      break;
    case LEFT:
      cell2.x -= w;
      break;
    default:
      break;
  }

  int curr_x = location.x - w / 4;
  int curr_y = location.y - w / 4;

  if ((cell1.x == curr_x && cell1.y == curr_y) ||
      (cell2.x == curr_x && cell2.y == curr_y))
    return true;
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