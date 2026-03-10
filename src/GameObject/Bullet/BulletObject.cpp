#include "BulletObject.h"
#include "Engine.h"
#include "Events/MovementEvent.h"
#include "Events/EventManager.h"

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
  EventManager& em = Engine::get().getEventManager();
  em.enqueue(MovementEvent(GameObject::getId(), m_speed * m_directionVector, true));

  m_timeToKill -= dt;
  if (m_timeToKill < 0) {
    GameObject::markObjectForDestruction();
    Engine::get().destroyObject(GameObject::getId());
  }
}

void BulletObject::shutdown()
{

  GameObject::shutdown();
}
