#include "Collision.h"
#include "Engine.h"
#include <iostream>

CollisionInfo::CollisionInfo(GameObject* aObject, GameObject* bObject, const std::string& collisionType)
  : aGameObject(aObject), bGameObject(bObject), type(collisionType), collision(true) {
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
GameObject* Collision::getOwner()
{
  if (auto p = Engine::get().getGameObjectById(m_owner).lock()) {
    return p.get();
  }
  else {
    std::cout << "Error in Collision::getOwner, owner does not exist." << std::endl;
    return nullptr;
  }
}
uint64_t Collision::getOwnerId()
{
  return m_owner;
}
std::string Collision::getType()
{
  return m_type;
}