#include "BulletObject.h"
#include "Engine.h"
#include "Constants.h"
#include "GameObject/Transform.h"

static const char* BULLET_ASSET = "assets/textures/bullet.png";

BulletObject::BulletObject(uint64_t id, const std::string& name, glm::vec2 directionVector, float speed)
  : GameObject(id, name, BULLET_ASSET), m_directionVector(directionVector), m_speed(speed)
{

}

BulletObject::~BulletObject()
{

}

void BulletObject::initialize()
{
  GameObject::initialize();
}

void BulletObject::tick(float dt)
{
  GameObject::tick(dt);

  glm::vec2 pos = getTransform()->getTranslation();
  pos += m_directionVector * m_speed * dt;
  getTransform()->setTranslation(pos);

  m_timeToKill -= dt;
  if (m_timeToKill < 0) {
    markObjectForDestruction();
    Engine::get().destroyObject(getId());
  }
}

void BulletObject::shutdown()
{

  GameObject::shutdown();
}
