#ifndef MOVEMENT_EVENT_H
#define MOVEMENT_EVENT_H
#include <cstdint>

// The axis/kind of movement this event applies.
//   Horizontal  — overwrite the X component of velocity; Y (gravity) is preserved.
//   Jump        — set a positive Y impulse; only fires when the body is grounded.
//   Fall        — force a fast-fall by overwriting Y velocity; X is preserved.
enum class MovementType
{
  Horizontal,
  Jump,
  Fall
};

class MovementEvent
{
public:
  // value semantics:
  //   Horizontal : target X velocity (+MOVE_SPEED, -MOVE_SPEED, or 0 to stop)
  //   Jump       : upward Y impulse magnitude (positive)
  //   Fall       : downward Y speed magnitude (will be negated internally)
  MovementEvent(uint64_t ownerId, MovementType type, float value);

  // Callable so it can be stored directly as std::function<void()>
  void operator()() const;

private:
  uint64_t     m_ownerId;
  MovementType m_type;
  float        m_value;
};

#endif
