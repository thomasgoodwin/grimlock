#ifndef PHYSICS_COMPONENT_CLASS_H
#define PHYSICS_COMPONENT_CLASS_H
#include <glm/glm.hpp>
#include <string>
#include <memory>

enum class BodyType {
  Static,
  Dynamic,
  Kinematic
};

class GameObject;

class PhysicsComponent
{
public:
  PhysicsComponent(uint64_t owner, BodyType type);
  BodyType getType() const;
  GameObject* getOwner() const;
  uint64_t getOwnerId() const;
  glm::vec2 getVelocity() const;
  void setVelocity(glm::vec2& velocity);
  bool getIsGrounded() const;
  void setIsGrounded(bool isGrounded);
  bool isStatic() const;
  bool isDynamic() const;
private:
  glm::vec2 m_velocity;
  uint64_t m_owner;
  BodyType m_type = BodyType::Static;
  bool m_isGrounded = false;
};

#endif