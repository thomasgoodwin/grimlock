#ifndef MELEE_BEHAVIOR_H
#define MELEE_BEHAVIOR_H
#include "AI/BehaviorComponent.h"
#include <cstdint>
#include <glm/glm.hpp>

class MeleeBehavior : public BehaviorComponent
{
public:
  MeleeBehavior(uint64_t ownerId, uint64_t targetId, float speed = 3.0f);

  void tick(float dt) override;
  void notifyHit() override;

private:
  uint64_t m_targetId;
  float m_speed;
  float m_damage = 10.0f;
  float m_range = 2.0f;
  bool m_isAttacking = false;
  bool m_facingLeft = false;
  float m_visionRange = 12.0f;
  float m_coneHalfAngle = 40.0f;    // degrees, forward cone
  float m_backVisionRange = 5.0f;   // short range behind the enemy
  glm::vec2 m_lastSeenPos = glm::vec2(0.0f);
  bool m_hasSeenTarget = false;
  uint64_t m_hurtBoxId = 0;
  float m_hurtBoxDelay = 0.4f;
  float m_hurtBoxTimer = 0.0f;
};

#endif
