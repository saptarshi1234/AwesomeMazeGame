#include "sounds.hpp"

std::map<SoundId, Mix_Chunk*> SoundManager::sound_map;

void SoundManager::initialize() {
  sound_map[SoundId::DEATH] = Mix_LoadWAV("res/sounds/death.wav");
  sound_map[SoundId::FIRE] = Mix_LoadWAV("res/sounds/fire.wav");
  sound_map[SoundId::POWERUP_FX] = Mix_LoadWAV("res/sounds/powerup.wav");
}

Mix_Chunk* SoundManager::getSound(SoundId id) { return sound_map[id]; }
