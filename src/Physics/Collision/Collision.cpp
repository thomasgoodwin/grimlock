#include "Collision.h"

CollisionInfo::CollisionInfo(const glm::vec2& collisionLocation, const std::string& collisionType)
  : location(collisionLocation), type(collisionType), collision(true) {
}

CollisionInfo::CollisionInfo(bool isColliding) : collision(isColliding) {}


Collision::Collision(GameObject* owner, const std::string& type, bool isStatic) : m_type(type), m_owner(m_owner), m_isStatic(isStatic) {}

bool Collision::getIsStatic() const
{
  return m_isStatic;
}
void Collision::setIsStatic(bool isStatic)
{
  m_isStatic = isStatic;
}
bool Collision::getIsTrigger() const
{
  return m_isTrigger;
}
void Collision::setIsTrigger(bool isTrigger)
{
  m_isTrigger = isTrigger;
}
GameObject* Collision::getOwner()
{
  return m_owner;
}
std::string Collision::getType()
{
  return m_type;
}