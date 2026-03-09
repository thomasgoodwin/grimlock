#ifndef COLLISION_CLASS_H
#define COLLISION_CLASS_H
#include <glm/glm.hpp>
#include <string>
#include <memory>

class GameObject;
class Transform;

struct CollisionInfo {
  CollisionInfo(GameObject* aObject, GameObject* bObject, const std::string& collisionType);
  CollisionInfo(bool isColliding);
  GameObject* aGameObject;
  GameObject* bGameObject;
  std::string type;
  bool collision;
};

class Collision
{
public:
  Collision(uint64_t owner, const std::string& type, bool isStatic = false);
  bool getIsStatic() const;
  void setIsStatic(bool isStatic);
  bool getIsTrigger() const;
  void setIsTrigger(bool isTrigger);
  GameObject* getOwner();
  std::string getType();
  uint64_t getOwnerId();

  // Override the collision box size. When set, physics and debug rendering use
  // this instead of the owner's transform scale.
  void setColliderSize(const glm::vec2& size);
  glm::vec2 getColliderSize() const;
  bool hasColliderSizeOverride() const;

  // Shift the collision box center relative to the sprite's transform position.
  // Useful when the character is not centered in its sprite frame.
  void setColliderOffset(const glm::vec2& offset);
  glm::vec2 getColliderOffset() const;

  // Returns the world-space center of the collision box for this frame.
  glm::vec2 getColliderCenter() const;

private:
  bool m_isStatic = false;
  bool m_isTrigger = false;
  uint64_t m_owner;
  std::string m_type = "";
  glm::vec2 m_sizeOverride = glm::vec2(-1.0f);
  glm::vec2 m_offset       = glm::vec2(0.0f);
};

#endif