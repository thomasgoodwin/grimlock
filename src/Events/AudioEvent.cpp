#include "AudioEvent.h"
#include "Audio/AudioManager.h"
#include "Engine.h"

AudioEvent::AudioEvent(const std::string& soundName)
  : m_soundName(soundName)
{
}

void AudioEvent::operator()() const
{
  Engine::get().getAudioManager().playSound(m_soundName);
}
