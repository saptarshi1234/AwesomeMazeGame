#include "sounds.hpp"

#include <SDL2/SDL_mixer.h>

std::map<SoundId, Mix_Chunk*> SoundManager::sound_map;
bool SoundManager::sound_on;

void SoundManager::initialize() {
  sound_map[SoundId::DEATH] = Mix_LoadWAV("res/sounds/death.wav");
  sound_map[SoundId::FIRE] = Mix_LoadWAV("res/sounds/fire.wav");
  sound_map[SoundId::POWERUP_FX] = Mix_LoadWAV("res/sounds/powerup.wav");
  sound_on = true;
}

void SoundManager::playOnce(SoundId id) {
  if (sound_on) Mix_PlayChannel(-1, sound_map[id], 0);
}

Mix_Chunk* SoundManager::getSound(SoundId id) { return sound_map[id]; }
