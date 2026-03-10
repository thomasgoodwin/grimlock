#include "MovementEvent.h"
#include "Physics/PhysicsComponent.h"
#include "Physics/PhysicsManager.h"
#include "Engine.h"

MovementEvent::MovementEvent(uint64_t ownerId, glm::vec2 value, bool setDirectly)
  : m_ownerId(ownerId), m_value(value), m_setDirectly(setDirectly)
{
}

void MovementEvent::operator()() const
{
  PhysicsComponent* physics = Engine::get().getPhysicsManager().getPhysicsComponent(m_ownerId);
  if (!physics)
    return;

  glm::vec2 velocity = physics->getVelocity();
  if (m_setDirectly)
    velocity.x = m_value.x; // for player input
  else
    velocity += m_value; // for jumps and falls

  physics->setVelocity(velocity);
}
