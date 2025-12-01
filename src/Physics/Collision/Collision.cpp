#include "Collision.h"
#include "Engine.h"
#include <iostream>

CollisionInfo::CollisionInfo(const glm::vec2& collisionLocation, const std::string& collisionType)
  : location(collisionLocation), type(collisionType), collision(true) {
}

CollisionInfo::CollisionInfo(bool isColliding) : collision(isColliding) {}


Collision::Collision(uint64_t owner, const std::string& type, bool isStatic) : m_owner(owner), m_type(type), m_isStatic(isStatic) {}

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
std::shared_ptr<GameObject> Collision::getOwner()
{
  
  if (auto p = Engine::get().getGameObjectById(m_owner).lock()) {
    return p;
  }
  else {
    std::cout << "Error in Collision::getOwner, owner does not exist." << std::endl;
    return nullptr;
  }
}
std::string Collision::getType()
{
  return m_type;
}