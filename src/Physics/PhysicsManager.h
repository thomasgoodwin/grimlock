#ifndef PHYSICS_MANAGER_H
#define PHYSICS_MANAGER_H
#include <vector>
#include <memory>

class Collision;
class CollisionInfo;
class GameObject;

class PhysicsManager
{
public:
  PhysicsManager();
  ~PhysicsManager();
  void initialize();
  void tick(float dt);
  void render();
  void shutdown();
  CollisionInfo checkCollision(Collision& collider1, Collision& collider2);
  void resolveCollision(Collision& collider1, Collision& collider2, CollisionInfo& info);
  void registerCollisionComponent(GameObject* owner, const std::string type, bool isStatic = false);
private:
  std::vector<std::unique_ptr<Collision>> m_dynamicColliders;
  std::vector<std::unique_ptr<Collision>> m_staticColliders;
};

#endif