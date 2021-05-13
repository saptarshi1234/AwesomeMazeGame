#include "textures.hpp"

void TextureManager::initialize(WindowManager win) {
  tex_map.insert(
      {Textures::BULLET, win.loadTexture("res/textures/bullets.png")});
  tex_map.insert(
      {Textures::GROUND, win.loadTexture("res/textures/ground.png")});
  tex_map.insert(
      {Textures::PLAYER, win.loadTexture("res/textures/all_tanks.png")});
}

SDL_Texture* TextureManager::getTex(Textures tex) { return tex_map.at(tex); }