#include "Collision/Collision.h"
#include "PhysicsManager.h"
#include "GameObject/GameObject.h"
#include <iostream>

PhysicsManager::PhysicsManager()
{

}
PhysicsManager::~PhysicsManager()
{

}
void PhysicsManager::initialize()
{

}
void PhysicsManager::tick(float dt)
{
  // use n^2 for now and manage isTrigger
  for (size_t i = 0; i < m_dynamicColliders.size(); i++) {
    for (size_t j = i + 1; j < m_dynamicColliders.size(); j++) {
      if (i == j) {
        continue;
      }
      CollisionInfo info = checkCollision(*m_dynamicColliders[i], *m_dynamicColliders[j]);
      if (info.collision) {
        resolveCollision(*m_dynamicColliders[i], *m_dynamicColliders[j], info);
      }
    }
    m_dynamicColliders[i]->getOwner()->applyForces(dt);
  }
  for (size_t i = 0; i < m_staticColliders.size(); i++) {
    for (size_t j = i + 1; j < m_staticColliders.size(); j++) {
      if (i == j) {
        continue;
      }
      CollisionInfo info = checkCollision(*m_staticColliders[i], *m_staticColliders[j]);
      if (info.collision) {
        resolveCollision(*m_dynamicColliders[i], *m_dynamicColliders[j], info);
      }
    }
    m_staticColliders[i]->getOwner()->applyForces(dt);
  }
  for (const auto& collider1 : m_dynamicColliders) {
    for (const auto& collider2 : m_staticColliders) {
      CollisionInfo info = checkCollision(*collider1, *collider2);
      if (info.collision) {
        resolveCollision(*collider1, *collider2, info);
      }
    }
  }
}
void PhysicsManager::registerCollisionComponent(uint64_t owner, const std::string type, bool isStatic)
{
  if (isStatic) {
    m_staticColliders.push_back(std::make_unique<Collision>(owner, type, isStatic));
  } else {
    m_dynamicColliders.push_back(std::make_unique<Collision>(owner, type, isStatic));
  }
}
CollisionInfo PhysicsManager::checkCollision(Collision& collider1, Collision& collider2)
{
  if (collider1.getType() == "box" && collider2.getType() == "box") {
    Transform& transform1 = *collider1.getOwner()->getTransform();
    Transform& transform2 = *collider2.getOwner()->getTransform();
    bool collisionX = transform1.getTranslation().x + transform1.getScale().x >= transform2.getTranslation().x &&
      transform2.getTranslation().x + transform2.getScale().x >= transform1.getTranslation().x;
    bool collisionY = transform1.getTranslation().y + transform1.getScale().y >= transform2.getTranslation().y &&
      transform2.getTranslation().y + transform2.getScale().y >= transform1.getTranslation().y;
    if (collisionX && collisionY) {
      return CollisionInfo((transform1.getTranslation() + transform2.getTranslation()) / 2.0f, "box/box");
    }
  }
  return CollisionInfo(false);
}
void PhysicsManager::resolveCollision(Collision& collider1, Collision& collider2, CollisionInfo& info)
{
  std::cout << "Collision detected!" << info.location.x << info.location.y << std::endl;
}
void PhysicsManager::render()
{

}
void PhysicsManager::shutdown()
{

}