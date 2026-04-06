#include "HurtBoxObject.h"
#include "Engine.h"
#include "Physics/PhysicsManager.h"
#include "Physics/Collision/Collision.h"

static const char* DUMMY_SPRITE = "assets/pixel-asset-pack/Enemies/Alien6.png";

HurtBoxObject::HurtBoxObject(uint64_t id, const std::string& name, uint64_t ownerEnemyId, float damage, float duration, int triggerCount, float hitCooldown)
  : GameObject(id, name, DUMMY_SPRITE), m_ownerEnemyId(ownerEnemyId), m_damage(damage), m_duration(duration), m_triggerCount(triggerCount), m_hitCooldown(hitCooldown)
{
}

void HurtBoxObject::initialize()
{
  uint64_t selfId = getId();
  bool ownerIsHostile = true;
  if (auto owner = Engine::get().getGameObjectById(m_ownerEnemyId).lock())
    ownerIsHostile = owner->isHostile();

  float damage = m_damage;

  Engine::get().getPhysicsManager().registerCollisionComponent(selfId, "box");
  Engine::get().getPhysicsManager().setColliderSize(selfId, glm::vec2(1.2f, 1.0f));

  if (Collision* col = Engine::get().getPhysicsManager().getCollisionComponent(selfId)) {
    col->setIsTrigger(true);
    col->setOnHit([this, selfId, ownerIsHostile, damage](uint64_t otherId) {
      if (m_triggerCount <= 0 || m_cooldownRemaining > 0.0f)
        return;
      if (auto other = Engine::get().getGameObjectById(otherId).lock()) {
        if (other->getHealth() && other->isHostile() != ownerIsHostile) {
          other->takeDamage(damage);
          m_cooldownRemaining = m_hitCooldown;
          --m_triggerCount;
          if (m_triggerCount <= 0)
            markObjectForDestruction();
        }
      }
    });
  }
}

void HurtBoxObject::tick(float dt)
{
  if (m_cooldownRemaining > 0.0f)
    m_cooldownRemaining -= dt;

  m_elapsed += dt;
  if (m_elapsed >= m_duration)
    markObjectForDestruction();
}

void HurtBoxObject::shutdown()
{
  Engine::get().getPhysicsManager().unregisterObject(getId());
  GameObject::shutdown();
}
