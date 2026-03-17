#ifndef MELEE_BEHAVIOR_H
#define MELEE_BEHAVIOR_H
#include "AI/BehaviorComponent.h"
#include <cstdint>

class MeleeBehavior : public BehaviorComponent
{
public:
  MeleeBehavior(uint64_t ownerId, uint64_t targetId, float speed = 3.0f);

  void tick(float dt) override;

private:
  uint64_t m_targetId;
  float m_speed;
  float m_range = 2.0f;
  bool m_isAttacking = false;
};

#endif
