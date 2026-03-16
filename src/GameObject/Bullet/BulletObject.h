#ifndef BULLET_OBJECT_H
#define BULLET_OBJECT_H
#include <string>
#include "GameObject/GameObject.h"

class BulletObject : public GameObject
{
public:
  BulletObject(uint64_t id, const std::string& name, glm::vec2 directionVector, float speed);
  ~BulletObject();

  void initialize() override;
  void tick(float dt) override;
  void shutdown() override;

  const std::string getTypename() const override { return "BulletObject"; };
  const bool getIsHostile() const { return m_hostile; };
  void setIsHostile(bool isHostile) { m_hostile = isHostile; };
  const float getDamage() const { return m_damage; };
  void setDamage(float damage) { m_damage = damage; };

private:
  float m_timeToKill = 9.0f;
  glm::vec2 m_directionVector = { 1.0f, 0.0f };
  float m_speed = 1.0f;
  bool m_hostile = false;
  float m_damage = 10.0f;
};

#endif