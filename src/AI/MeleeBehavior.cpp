#include "AI/MeleeBehavior.h"
#include "Engine.h"
#include "GameObject/GameObject.h"
#include "Physics/PhysicsManager.h"
#include "Physics/PhysicsComponent.h"
#include "GameObject/HurtBoxObject/HurtBoxObject.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <cmath>

MeleeBehavior::MeleeBehavior(uint64_t ownerId, uint64_t targetId, float speed)
  : BehaviorComponent(ownerId), m_targetId(targetId), m_speed(speed)
{
}

void MeleeBehavior::notifyHit()
{
  if (auto target = Engine::get().getGameObjectById(m_targetId).lock()) {
    m_lastSeenPos = target->getTransform()->getTranslation();
    m_hasSeenTarget = true;
  }
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
  float dy = targetPos.y - ownerPos.y;
  glm::vec2 velocity = physics->getVelocity();

  auto animation = owner->getAnimatedSprite();

  float dist = glm::length(glm::vec2(dx, dy));
  float facingDir = m_facingLeft ? -1.0f : 1.0f;

  bool playerBehind = (dx * facingDir) < 0.0f;
  if (playerBehind && dist < m_backVisionRange && glm::abs(dy) < m_backVisionRange * 0.3f) {
    m_facingLeft = !m_facingLeft;
    facingDir = -facingDir;
    if (animation) {
      animation->setFlipX(m_facingLeft);
    }
  }

  float angleToTarget = glm::degrees(std::atan2(dy, dx * facingDir));
  bool inCone = dist < m_visionRange && glm::abs(angleToTarget) < m_coneHalfAngle;

  if (inCone) {
    m_lastSeenPos = targetPos;
    m_hasSeenTarget = true;
  }

  if (m_isAttacking) {
    velocity.x = 0.0f;
    if (!m_hurtBoxId) {
      m_hurtBoxTimer += dt;
      if (m_hurtBoxTimer >= m_hurtBoxDelay) {
        m_hurtBoxId = Engine::get().addGameObject<HurtBoxObject>("hurtbox", m_ownerId, m_damage, 0.1f, 1, 0.5f);
        if (auto hurtBox = Engine::get().getGameObjectById(m_hurtBoxId).lock()) {
          float offset = m_facingLeft ? -1.0f : 1.0f;
          hurtBox->getTransform()->setTranslation(ownerPos + glm::vec2(offset, 0.0f));
          hurtBox->initialize();
        }
      }
    }
    else {
      // keep hurt box in front of the enemy
      if (auto hurtBox = Engine::get().getGameObjectById(m_hurtBoxId).lock()) {
        float offset = m_facingLeft ? -1.0f : 1.0f;
        hurtBox->getTransform()->setTranslation(ownerPos + glm::vec2(offset, 0.0f));
      }
    }
    if (animation && !animation->isEventMode()) {
      m_isAttacking = false;
      m_hurtBoxTimer = 0.0f;
      if (m_hurtBoxId) {
        Engine::get().destroyObject(m_hurtBoxId);
        m_hurtBoxId = 0;
      }
      animation->setCyclingAnimation("idle");
    }
  }
  else if (!inCone) {
    if (m_hasSeenTarget) {
      float ldx = m_lastSeenPos.x - ownerPos.x;
      if (glm::abs(ldx) > m_range * 0.5f) {
        m_facingLeft = ldx < 0.0f;
        velocity.x = (m_facingLeft ? -1.0f : 1.0f) * m_speed;
        if (animation) {
          animation->setFlipX(m_facingLeft);
          animation->setCyclingAnimation("walk");
        }
      }
      else {
        velocity.x = 0.0f;
        if (animation) animation->setCyclingAnimation("idle");
      }
    }
    else {
      velocity.x = 0.0f;
      if (animation) animation->setCyclingAnimation("idle");
    }
  }
  else if (glm::abs(dx) < m_range) {
    velocity.x = 0.0f;
    m_isAttacking = true;
    m_hurtBoxTimer = 0.0f;
    if (animation) {
      animation->setCyclingAnimation("idle");
      animation->triggerAnimation("attack");
    }
  }
  else {
    m_facingLeft = dx < 0.0f;
    velocity.x = (m_facingLeft ? -1.0f : 1.0f) * m_speed;
    if (animation) {
      animation->setFlipX(m_facingLeft);
      animation->setCyclingAnimation("walk");
    }
  }
  physics->setVelocity(velocity);
}
