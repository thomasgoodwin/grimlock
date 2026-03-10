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
#include <iostream>

CharacterController::CharacterController(uint64_t ownerId)
  : m_ownerId(ownerId)
{
}

void CharacterController::onHorizontalInput()
{
  bool moving = (m_leftHeld && !m_rightHeld) || (m_rightHeld && !m_leftHeld);
  bool facingLeft = m_leftHeld && !m_rightHeld;

  if (m_leftHeld && !m_rightHeld)
    m_direction = "left";
  else if (m_rightHeld && !m_leftHeld)
    m_direction = "right";

  uint64_t id = m_ownerId;
  Engine::get().getEventManager().enqueue([id, moving, facingLeft]() {
    if (auto obj = Engine::get().getGameObjectById(id).lock())
      if (auto anim = obj->getAnimatedSprite())
      {
        if (moving)
          anim->setFlipX(facingLeft);
        anim->setCyclingAnimation(moving ? "run" : "idle");
      }
    });
}

void CharacterController::initialize()
{
  EventManager& em = Engine::get().getEventManager();

  em.bindKey(GLFW_KEY_A, GLFW_PRESS, [this]() { m_leftHeld = true;  onHorizontalInput(); });
  em.bindKey(GLFW_KEY_A, GLFW_RELEASE, [this]() { m_leftHeld = false; onHorizontalInput(); });

  em.bindKey(GLFW_KEY_D, GLFW_PRESS, [this]() { m_rightHeld = true;  onHorizontalInput(); });
  em.bindKey(GLFW_KEY_D, GLFW_RELEASE, [this]() { m_rightHeld = false; onHorizontalInput(); });

  uint64_t id = m_ownerId;

  em.bindKey(GLFW_KEY_W, GLFW_PRESS, [id]() {
    PhysicsComponent* physics = Engine::get().getPhysicsManager().getPhysicsComponent(id);
    if (physics->getIsGrounded()) {
      Engine::get().getEventManager().enqueue(MovementEvent(id, glm::vec2{ 0.0f, JUMP_IMPULSE }));
      Engine::get().getEventManager().enqueue([id]() {
        if (auto obj = Engine::get().getGameObjectById(id).lock())
          if (auto anim = obj->getAnimatedSprite())
            anim->triggerAnimation("jump");
        });
    }
    });

  em.bindKey(GLFW_KEY_S, GLFW_PRESS,
    [id]()
    {
      Engine::get().getEventManager().enqueue(MovementEvent(id, glm::vec2{ 0.0f, -FALL_SPEED }));
    }
  );
}

void CharacterController::tick(float dt)
{
  PhysicsComponent* physics = Engine::get().getPhysicsManager().getPhysicsComponent(m_ownerId);
  if (!physics)
    return;

  glm::vec2 velocity = physics->getVelocity();

  if (m_leftHeld && !m_rightHeld)
  {
    velocity.x = -MOVE_SPEED;
    physics->setHasMovementInput(true);
  }
  else if (m_rightHeld && !m_leftHeld)
  {
    velocity.x = MOVE_SPEED;
    physics->setHasMovementInput(true);
  }
  else
  {
    physics->setHasMovementInput(false);
  }

  physics->setVelocity(velocity);
}

void CharacterController::shutdown()
{
  
}
