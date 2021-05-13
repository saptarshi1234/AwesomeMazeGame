#include "textures.hpp"
std::map<TextureID, SDL_Texture*> TextureManager::tex_map;

void TextureManager::initialize(WindowManager win) {
  tex_map[TextureID::BULLET] =
      win.loadTexture("res/textures/tanks/others/bullets.png");
  tex_map[TextureID::GUN] =
      win.loadTexture("res/textures/tanks/others/guns.png");
  tex_map[TextureID::GROUND] =
      win.loadTexture("res/textures/background/ground.png");

  // TODO
  tex_map[TextureID::TANK_F1] =
      win.loadTexture("res/textures/tanks/body/HULL_F1.png");
  tex_map[TextureID::TANK_F2] =
      win.loadTexture("res/textures/tanks/body/HULL_F2.png");
}

SDL_Texture* TextureManager::getTex(TextureID tex) { return tex_map[tex]; }