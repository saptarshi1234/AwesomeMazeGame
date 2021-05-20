#pragma once
#include <map>

#include "SDL2/SDL_mixer.h"

enum SoundId { FIRE = 0, DEATH, POWERUP_FX };

class SoundManager {
 public:
  static std::map<SoundId, Mix_Chunk*> sound_map;
  static void initialize();
  static Mix_Chunk* getSound(SoundId id);

  static bool sound_on;
  static void playOnce(SoundId);
};