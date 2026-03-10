#ifndef MOVEMENT_EVENT_H
#define MOVEMENT_EVENT_H
#include <cstdint>

enum class MovementType
{
  Horizontal,
  Jump,
  Fall
};

class MovementEvent
{
public:
  MovementEvent(uint64_t ownerId, MovementType type, float value);
  void operator()() const;

private:
  uint64_t     m_ownerId;
  MovementType m_type;
  float        m_value;
};

#endif
