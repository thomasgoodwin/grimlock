#include <iostream>
#include "Collision/Collision.h"
#include "PhysicsManager.h"
#include "GameObject/GameObject.h"
#include "Constants.h"
#include "GameObject/Transform.h"

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
  for (const auto& physics : m_physicsComponents) {
    if (physics->isDynamic()) {
      glm::vec2 velocity = physics->getVelocity();
      velocity.y -= GRAVITY * dt;
      physics->setVelocity(velocity);
    }
  }
  for (const auto& collider : m_dynamicColliders) {
    collider->getOwner()->applyForces(dt);
  }
  // use n^2 for now and manage isTrigger
  for (int i = 0; i < m_dynamicColliders.size(); i++) {
    for (int j = i + 1; j < m_dynamicColliders.size(); j++) {
      CollisionInfo info = checkCollision(*m_dynamicColliders[i], *m_dynamicColliders[j]);
      if (info.collision) {
        resolveCollision(*m_dynamicColliders[i], *m_dynamicColliders[j], info);
      }
    }
  }
  for (const auto& aCollider : m_dynamicColliders) {
    for (const auto& bCollider : m_staticColliders) {
      CollisionInfo info = checkCollision(*aCollider, *bCollider);
      if (info.collision) {
        resolveCollision(*aCollider, *bCollider, info);
      }
    }
  }
}
void PhysicsManager::registerCollisionComponent(uint64_t owner, const std::string type, bool isStatic)
{
  if (isStatic) {
    m_staticColliders.push_back(std::make_unique<Collision>(owner, type, isStatic));
  }
  else {
    m_dynamicColliders.push_back(std::make_unique<Collision>(owner, type, isStatic));
  }
}
void PhysicsManager::registerPhysicsComponent(uint64_t owner, BodyType type)
{
  m_physicsComponents.push_back(std::make_unique<PhysicsComponent>(owner, type));
}
CollisionInfo PhysicsManager::checkCollision(Collision& aCollider, Collision& bCollider)
{
  if (aCollider.getType() == "box" && bCollider.getType() == "box") {
    Transform& transform1 = *aCollider.getOwner()->getTransform();
    Transform& transform2 = *bCollider.getOwner()->getTransform();
    bool collisionX = transform1.getTranslation().x + transform1.getScale().x >= transform2.getTranslation().x &&
      transform2.getTranslation().x + transform2.getScale().x >= transform1.getTranslation().x;
    bool collisionY = transform1.getTranslation().y + transform1.getScale().y >= transform2.getTranslation().y &&
      transform2.getTranslation().y + transform2.getScale().y >= transform1.getTranslation().y;
    if (collisionX && collisionY) {
      return CollisionInfo(aCollider.getOwner(), aCollider.getOwner(), "box/box");
    }
  }
  return CollisionInfo(false);
}
void PhysicsManager::resolveCollision(Collision& aCollider, Collision& bCollider, CollisionInfo& info)
{
  PhysicsComponent* aPhysics = getPhysicsComponent(aCollider.getOwnerId());
  PhysicsComponent* bPhysics = getPhysicsComponent(bCollider.getOwnerId());

  auto aTransform = info.aGameObject->getTransform();
  auto bTransform = info.bGameObject->getTransform();

  glm::vec2 aPos = aTransform->getTranslation();
  glm::vec2 bPos = bTransform->getTranslation();

  glm::vec2 aHalf = aTransform->getScale() * 0.5f;
  glm::vec2 bHalf = bTransform->getScale() * 0.5f;

  float dx = aPos.x - bPos.x;
  float px = (aHalf.x + bHalf.x) - std::abs(dx);
  float dy = aPos.y - bPos.y;
  float py = (aHalf.y + bHalf.y) - std::abs(dy);

  if (px <= 0 || py <= 0) {
    return;
  }

  bool resolveX = px < py;

  float aMove = 1.0f;
  float bMove = 0.0f;

  std::cout << "px=" << px << " py=" << py
    << " aHalf=" << aHalf.y
    << " bHalf=" << bHalf.y
    << std::endl;
  if (aPhysics && bPhysics) {
    if (aPhysics->isDynamic() && bPhysics->isDynamic()) {
      aMove = 0.5f;
      bMove = 0.5f;
    }
    else if (aPhysics->isDynamic() && bPhysics->isStatic()) {
      aMove = 1.0f;
      bMove = 0.0f;
    }
    else if (aPhysics->isStatic() && bPhysics->isDynamic()) {
      aMove = 0.0f;
      bMove = 1.0f;
    }
    else {
      return;
    }
  }
  else if (aPhysics && !bPhysics) {
    aMove = 1.0f;
    bMove = 0.0f;
  }
  else if (!aPhysics && bPhysics) {
    aMove = 0.0f;
    bMove = 1.0f;
  }
  else {
    return;
  }

  if (resolveX) {
    std::cout << resolveX << std::endl;
    float sx = (dx < 0 ? -px : px);

    aPos.x += sx * aMove;
    bPos.x -= sx * bMove;

    if (aPhysics && aPhysics->isDynamic()) {
      aPhysics->setVelocity(glm::vec2({ 0.0f, aPhysics->getVelocity().y }));
    }
    if (bPhysics && bPhysics->isDynamic()) {
      bPhysics->setVelocity(glm::vec2({ 0.0f, bPhysics->getVelocity().y }));
    }
  }
  else {
    float sy = (dy < 0 ? -py : py);
    aPos.y += sy * aMove;
    bPos.y -= sy * bMove;

    if (aPhysics && aPhysics->isDynamic()) {
      glm::vec2 velocity = aPhysics->getVelocity();
      velocity.y = 0;
      aPhysics->setVelocity(velocity);
      if (sy < 0) {
        std::cout << sy << std::endl;
        aPhysics->setIsGrounded(true);
      }
    }
    if (bPhysics && bPhysics->isDynamic()) {
      glm::vec2 velocity = bPhysics->getVelocity();
      velocity.y = 0;
      bPhysics->setVelocity(velocity);
      if (sy > 0) {
        std::cout << sy << std::endl;
        bPhysics->setIsGrounded(true);
      }
    }
  }
  aTransform->setTranslation(aPos);
  bTransform->setTranslation(bPos);
}
Collision* PhysicsManager::getCollisionComponent(uint64_t owner) const
{
  for (int i = 0; i < m_dynamicColliders.size(); i++) {
    if (m_dynamicColliders[i]->getOwnerId() == owner) {
      return m_dynamicColliders[i].get();
    }
  }
  for (int i = 0; i < m_staticColliders.size(); i++) {
    if (m_staticColliders[i]->getOwnerId() == owner) {
      return m_staticColliders[i].get();
    }
  }
  return nullptr;
}
PhysicsComponent* PhysicsManager::getPhysicsComponent(uint64_t owner) const
{
  for (int i = 0; i < m_physicsComponents.size(); i++) {
    if (m_physicsComponents[i]->getOwnerId() == owner) {
      return m_physicsComponents[i].get();
    }
  }
  return nullptr;
}
void PhysicsManager::render()
{

}
void PhysicsManager::shutdown()
{

}