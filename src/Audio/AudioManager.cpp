#include "AudioManager.h"
#include <fstream>
#include <iostream>
#include "nlohmann/json.hpp"

AudioManager::AudioManager()
{
  FMOD_RESULT result;
  result = FMOD::System_Create(&m_system);
  if (result != FMOD_OK) {
    std::cerr << "FMOD System_Create failed: " << result << std::endl;
    return;
  }
  result = m_system->init(32, FMOD_INIT_NORMAL, nullptr);
  if (result != FMOD_OK) {
    std::cerr << "FMOD init failed: " << result << std::endl;
    m_system->release();
    m_system = nullptr;
  }
  
}
AudioManager::~AudioManager()
{

}
void AudioManager::initialize()
{
  if (!m_system) 
    return;

  std::ifstream file("assets/SoundBank.json");
  if (!file.is_open()) {
    std::cerr << "Failed to open SoundBank.json" << std::endl;
    return;
  }

  try {
    nlohmann::json data = nlohmann::json::parse(file);
    for (const auto& entry : data["sounds"]) {
      std::string name = entry["name"];
      std::string path = entry["path"];

      FMOD::Sound* sound = nullptr;
      FMOD_RESULT result = m_system->createSound(path.c_str(), FMOD_DEFAULT, nullptr, &sound);
      if (result != FMOD_OK) {
        std::cerr << "Failed to load sound: " << path << std::endl;
        continue;
      }
      m_soundBank[name] = sound;
    }
  }
  catch (const std::exception& e) {
    std::cerr << "SoundBank.json error: " << e.what() << std::endl;
  }
}
void AudioManager::tick(float dt)
{
  if (m_system)
    m_system->update();
}
void AudioManager::render()
{

}
void AudioManager::shutdown()
{

}
void AudioManager::playSound(const std::string& name)
{
  if (m_soundBank.find(name) != m_soundBank.end())
  {
    FMOD_RESULT result;
    m_system->playSound(m_soundBank[name], nullptr, false, &m_mainChannel);
    if (result != FMOD_OK) {
      std::cout << "Error playing sound: " << name << std::endl;
    }
  }
  else
  {
    std::cout << "Error playing sound: " << name << std::endl;
  }
}