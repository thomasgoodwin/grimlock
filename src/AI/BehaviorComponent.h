#ifndef BEHAVIOR_COMPONENT_H
#define BEHAVIOR_COMPONENT_H
#include <cstdint>

class BehaviorComponent
{
public:
  BehaviorComponent(uint64_t ownerId) : m_ownerId(ownerId) {}
  virtual ~BehaviorComponent() = default;

  virtual void initialize() {}
  virtual void tick(float dt) {}
  virtual void shutdown() {}

protected:
  uint64_t m_ownerId;
};

#endif
