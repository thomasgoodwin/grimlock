#ifndef PHYSICS_MANAGER_H
#define PHYSICS_MANAGER_H
#include <vector>
#include <memory>
#include "PhysicsComponent.h"

class Collision;
class CollisionInfo;
class GameObject;
class PhysicsComponent;

class PhysicsManager
{
public:
  PhysicsManager();
  ~PhysicsManager();
  void initialize();
  void tick(float dt);
  void render();
  void shutdown();
  CollisionInfo checkCollision(Collision& aCollider, Collision& bCollider);
  void resolveCollision(Collision& aCollider, Collision& bCollider, CollisionInfo& info);
  void registerCollisionComponent(uint64_t owner, const std::string type, bool isStatic = false);
  void registerPhysicsComponent(uint64_t owner, BodyType type);
  Collision* getCollisionComponent(uint64_t owner) const;
  PhysicsComponent* getPhysicsComponent(uint64_t owner) const;
private:
  std::vector<std::unique_ptr<Collision>> m_dynamicColliders;
  std::vector<std::unique_ptr<Collision>> m_staticColliders;
  std::vector<std::unique_ptr<PhysicsComponent>> m_physicsComponents;
};

#endif