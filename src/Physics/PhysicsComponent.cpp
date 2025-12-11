#include "PhysicsComponent.h"
#include "GameObject/GameObject.h"
#include "Engine.h"

PhysicsComponent::PhysicsComponent(uint64_t owner, BodyType type) : m_owner(owner), m_type(type)
{
  m_velocity = { 0.0f, 0.0f };
}
GameObject* PhysicsComponent::getOwner() const
{
  if (auto p = Engine::get().getGameObjectById(m_owner).lock()) {
    return p.get();
  }
  else {
    std::cout << "Error in PhysicsComponent::getOwner, owner does not exist." << std::endl;
    return nullptr;
  }
}
uint64_t PhysicsComponent::getOwnerId() const
{
  return m_owner;
}
BodyType PhysicsComponent::getType() const
{
  return m_type;
}
glm::vec2 PhysicsComponent::getVelocity() const
{
  return m_velocity;
}
void PhysicsComponent::setVelocity(glm::vec2& velocity)
{
  m_velocity = velocity;
}
bool PhysicsComponent::getIsGrounded() const
{
  return m_isGrounded;
}
void PhysicsComponent::setIsGrounded(bool isGrounded)
{
  m_isGrounded = isGrounded;
}
bool PhysicsComponent::isStatic() const
{
  return m_type == BodyType::Static;
}
bool PhysicsComponent::isDynamic() const
{
  return m_type == BodyType::Dynamic;
}