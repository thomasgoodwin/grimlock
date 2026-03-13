#ifndef AUDIO_EVENT_H
#define AUDIO_EVENT_H
#include <string>

class AudioEvent
{
public:
  AudioEvent(const std::string& soundName);
  void operator()() const;

private:
  std::string m_soundName;
};

#endif
