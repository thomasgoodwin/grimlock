#ifndef COLLISION_CLASS_H
#define COLLISION_CLASS_H
#include <glm/glm.hpp>
#include <string>
#include <memory>

class GameObject;

struct CollisionInfo {
  CollisionInfo(const glm::vec2& collisionLocation, const std::string& collisionType);
  CollisionInfo(bool isColliding);
  glm::vec2 location;
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
  std::shared_ptr<GameObject> getOwner();
  std::string getType();
private:
  bool m_isStatic = false;
  bool m_isTrigger = false;
  uint64_t m_owner;
  std::string m_type = "";
};

#endif