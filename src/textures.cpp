#include "textures.hpp"
std::map<TextureID, SDL_Texture*> TextureManager::tex_map;

void TextureManager::initialize(WindowManager win) {
  tex_map[TextureID::BULLET] =
      win.loadTexture("res/textures/tanks/others/bullets.png");
  tex_map[TextureID::GUN] =
      win.loadTexture("res/textures/tanks/others/guns.png");
  tex_map[TextureID::GROUND] =
      win.loadTexture("res/textures/background/ground.png");
  tex_map[TextureID::EXPLOSION] =
      win.loadTexture("res/textures/tanks/others/explosion.png");
  tex_map[TextureID::OVERLAY] =
      win.loadTexture("res/textures/background/overlay.png");

  // TODO
  tex_map[TextureID::TANK_F1] =
      win.loadTexture("res/textures/tanks/body/HULL_F1.png");
  tex_map[TextureID::TANK_F2] =
      win.loadTexture("res/textures/tanks/body/HULL_F2.png");
  tex_map[TextureID::TANK_INV] =
      win.loadTexture("res/textures/tanks/body/HULL_blurred.png");
  tex_map[TextureID::BOT_BODY] =
      win.loadTexture("res/textures/tanks/body/Bot_body2.png");
  tex_map[TextureID::GUN_INV] =
      win.loadTexture("res/textures/tanks/others/guns_blurred.png");

  tex_map[TextureID::CLIENT] =
      win.loadTexture("res/textures/background/client.png");
  tex_map[TextureID::SERVER] =
      win.loadTexture("res/textures/background/server.png");
  tex_map[TextureID::LOGO] =
      win.loadTexture("res/textures/background/logo2.png");
  tex_map[TextureID::HEALTH_BAR] =
      win.loadTexture("res/textures/tanks/others/health_bar.png");
  tex_map[TextureID::HEALTH_BODY] =
      win.loadTexture("res/textures/tanks/others/health_body.png");

  tex_map[TextureID::MULTIPLIER_2X] =
      win.loadTexture("res/textures/tanks/others/2x_Multiplier.png");
  tex_map[TextureID::INVISIBLE] =
      win.loadTexture("res/textures/tanks/others/inv.png");
  tex_map[TextureID::POWERUP] =
      win.loadTexture("res/textures/tanks/others/fire2.png");
  tex_map[TextureID::SHIELD] =
      win.loadTexture("res/textures/tanks/others/shield.png");
  tex_map[TextureID::HP] =
      win.loadTexture("res/textures/tanks/others/hp2.jpeg");

  tex_map[TextureID::SCORE] =
      win.loadTexture("res/textures/background/score.png");
  tex_map[TextureID::IP] = win.loadTexture("res/textures/background/ip.png");
}

SDL_Texture* TextureManager::getTex(TextureID tex) { return tex_map[tex]; }