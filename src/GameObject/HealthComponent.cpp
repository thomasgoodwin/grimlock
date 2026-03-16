#include "HealthComponent.h"
#include "Engine.h"
#include <algorithm>

HealthComponent::HealthComponent(uint64_t ownerId, float maxHp)
  : m_ownerId(ownerId), m_hp(maxHp), m_maxHp(maxHp)
{
}

void HealthComponent::takeDamage(float amount)
{
  m_hp = std::max(0.0f, m_hp - amount);
  if (m_hp <= 0.0f)
  {
    Engine::get().destroyObject(m_ownerId);
  }
}
