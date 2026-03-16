#ifndef HEALTH_COMPONENT_H
#define HEALTH_COMPONENT_H
#include <cstdint>

class HealthComponent
{
public:
  HealthComponent(uint64_t ownerId, float maxHp);

  void takeDamage(float amount);

  float getHp() const { return m_hp; }
  float getMaxHp() const { return m_maxHp; }
  bool isDead() const { return m_hp <= 0.0f; }

private:
  uint64_t m_ownerId;
  float m_hp;
  float m_maxHp;
};

#endif
