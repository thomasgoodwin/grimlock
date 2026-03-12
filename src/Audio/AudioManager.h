#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H
#include <unordered_map>
#include <string>
#include "fmod.hpp"

class AudioManager {
public:
  AudioManager();
  ~AudioManager();
  void initialize();
  void tick(float dt);
  void render();
  void shutdown();
  void playSound(const std::string& name);
private:
  std::unordered_map<std::string, FMOD::Sound*> m_soundBank;
  FMOD::System* m_system = nullptr;
  FMOD::Channel* m_mainChannel = nullptr;
};

#endif