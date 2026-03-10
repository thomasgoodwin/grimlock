#include "CharacterController.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Engine.h"
#include "Events/EventManager.h"
#include "Events/MovementEvent.h"
#include "GameObject/GameObject.h"
#include "Constants.h"
#include "Physics/PhysicsManager.h"
#include "Physics/PhysicsComponent.h"

CharacterController::CharacterController(uint64_t ownerId)
  : m_ownerId(ownerId)
{
}

void CharacterController::applyHorizontal()
{
  float vel      = 0.0f;
  bool  moving   = false;
  bool  facingLeft = false;

  if (m_leftHeld && !m_rightHeld)
  {
    vel = -MOVE_SPEED;
    moving = true;
    facingLeft = true;
  }
  else if (m_rightHeld && !m_leftHeld)
  {
    vel = MOVE_SPEED;
    moving = true;
    facingLeft = false;
  }
  // Both held or neither held → vel stays 0, moving stays false

  uint64_t id = m_ownerId;
  EventManager& em = Engine::get().getEventManager();

  em.enqueue(MovementEvent(id, MovementType::Horizontal, vel));
  em.enqueue([id, moving, facingLeft]() {
    if (auto obj = Engine::get().getGameObjectById(id).lock())
      if (auto anim = obj->getAnimatedSprite())
      {
        if (moving) anim->setFlipX(facingLeft);
        anim->setCyclingAnimation(moving ? "run" : "idle");
      }
  });
}

void CharacterController::initialize()
{
  EventManager& em = Engine::get().getEventManager();

  em.bindKey(GLFW_KEY_A, GLFW_PRESS,   [this]() { m_leftHeld = true;  applyHorizontal(); });
  em.bindKey(GLFW_KEY_A, GLFW_RELEASE, [this]() { m_leftHeld = false; applyHorizontal(); });

  em.bindKey(GLFW_KEY_D, GLFW_PRESS,   [this]() { m_rightHeld = true;  applyHorizontal(); });
  em.bindKey(GLFW_KEY_D, GLFW_RELEASE, [this]() { m_rightHeld = false; applyHorizontal(); });

  uint64_t id = m_ownerId;

  em.bindKey(GLFW_KEY_W, GLFW_PRESS, [id]() {
    PhysicsComponent* physics = Engine::get().getPhysicsManager().getPhysicsComponent(id);
    if (physics->getIsGrounded()) {
      Engine::get().getEventManager().enqueue(MovementEvent(id, MovementType::Jump, JUMP_IMPULSE));
      Engine::get().getEventManager().enqueue([id]() {
        if (auto obj = Engine::get().getGameObjectById(id).lock())
          if (auto anim = obj->getAnimatedSprite())
            anim->triggerAnimation("jump");
      });
    }
  });

  em.bindKey(GLFW_KEY_S, GLFW_PRESS, [id]() {
    Engine::get().getEventManager().enqueue(MovementEvent(id, MovementType::Fall, FALL_SPEED));
  });
}

void CharacterController::tick(float dt)
{
}

void CharacterController::shutdown()
{
}
