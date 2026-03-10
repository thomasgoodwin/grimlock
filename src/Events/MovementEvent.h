#ifndef MOVEMENT_EVENT_H
#define MOVEMENT_EVENT_H
#include <cstdint>
#include <glm/glm.hpp>

class MovementEvent
{
public:
  MovementEvent(uint64_t ownerId, glm::vec2 value, bool setDirectly = false);
  void operator()() const;

private:
  uint64_t m_ownerId;
  glm::vec2 m_value;
  bool m_setDirectly;
};

#endif
