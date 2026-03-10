#include "MovementEvent.h"
#include "Physics/PhysicsComponent.h"
#include "Physics/PhysicsManager.h"
#include "Engine.h"

MovementEvent::MovementEvent(uint64_t ownerId, MovementType type, float value)
  : m_ownerId(ownerId), m_type(type), m_value(value)
{
}

void MovementEvent::operator()() const
{
  PhysicsComponent* physics = Engine::get().getPhysicsManager().getPhysicsComponent(m_ownerId);
  if (!physics) 
    return;

  glm::vec2 velocity = physics->getVelocity();

  switch (m_type)
  {
    case MovementType::Horizontal:
      velocity.x = m_value;
      break;

    case MovementType::Jump:
      velocity.y = m_value;
      break;

    case MovementType::Fall:
      velocity.y = -m_value;
      break;
  }

  physics->setVelocity(velocity);
}
