#ifndef HURT_BOX_OBJECT_H
#define HURT_BOX_OBJECT_H
#include "GameObject/GameObject.h"
#include <cstdint>

class HurtBoxObject : public GameObject
{
public:
  HurtBoxObject(uint64_t id, const std::string& name, uint64_t ownerEnemyId, float damage, float duration, int triggerCount, float hitCooldown);

  void initialize() override;
  void tick(float dt) override;
  void render() override {}
  void shutdown() override;

  const std::string getTypename() const override { return "HurtBoxObject"; }

private:
  uint64_t m_ownerEnemyId;
  float m_damage;
  float m_duration;
  float m_elapsed = 0.0f;
  int m_triggerCount;
  float m_hitCooldown;
  float m_cooldownRemaining = 0.0f;
};

#endif
