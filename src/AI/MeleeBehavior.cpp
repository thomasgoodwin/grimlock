#include "AI/MeleeBehavior.h"
#include "Engine.h"
#include "GameObject/GameObject.h"
#include "Physics/PhysicsManager.h"
#include "Physics/PhysicsComponent.h"
#include <glm/glm.hpp>

MeleeBehavior::MeleeBehavior(uint64_t ownerId, uint64_t targetId, float speed)
  : BehaviorComponent(ownerId), m_targetId(targetId), m_speed(speed)
{
}

void MeleeBehavior::tick(float dt)
{
  auto ownerWeak = Engine::get().getGameObjectById(m_ownerId);
  auto targetWeak = Engine::get().getGameObjectById(m_targetId);

  auto owner = ownerWeak.lock();
  auto target = targetWeak.lock();

  if (!owner || !target) {
    return;
  }

  PhysicsComponent* physics = Engine::get().getPhysicsManager().getPhysicsComponent(m_ownerId);
  if (!physics) {
    return;
  }

  glm::vec2 ownerPos = owner->getTransform()->getTranslation();
  glm::vec2 targetPos = target->getTransform()->getTranslation();

  float dx = targetPos.x - ownerPos.x;
  glm::vec2 velocity = physics->getVelocity();

  auto anim = owner->getAnimatedSprite();

  if (m_isAttacking) {
    velocity.x = 0.0f;
    if (anim && !anim->isEventMode()) {
      m_isAttacking = false;
      anim->setCyclingAnimation("idle");
    }
  }
  else if (glm::abs(dx) < m_range) {
    velocity.x = 0.0f;
    m_isAttacking = true;
    if (anim) {
      anim->setCyclingAnimation("idle");
      anim->triggerAnimation("attack");
    }
  }
  else {
    bool movingLeft = dx < 0.0f;
    velocity.x = (movingLeft ? -1.0f : 1.0f) * m_speed;
    if (anim) {
      anim->setFlipX(movingLeft);
      anim->setCyclingAnimation("walk");
    }
  }
  physics->setVelocity(velocity);
}
