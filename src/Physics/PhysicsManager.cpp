#include <iostream>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Collision/Collision.h"
#include "PhysicsManager.h"
#include "GameObject/GameObject.h"
#include "Constants.h"
#include "GameObject/Transform.h"
#include "Graphics/GraphicsManager.h"
#include "Engine.h"

PhysicsManager::PhysicsManager()
{

}
PhysicsManager::~PhysicsManager()
{

}
void PhysicsManager::initialize()
{
  // Unit square outline — 4 corners, drawn with GL_LINE_LOOP
  float boxVerts[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.5f,  0.5f, 0.0f,
    -0.5f,  0.5f, 0.0f,
  };

  m_debugVAO = std::make_shared<VAO>();
  m_debugVBO = std::make_shared<VBO>(boxVerts, sizeof(boxVerts));

  m_debugVAO->bind();
  m_debugVBO->bind();
  m_debugVAO->linkAttribute(*m_debugVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
  m_debugVAO->unbind();

  m_debugShader = Engine::get().getGraphicsManager().addShader(
    "assets/shaders/debug.vert",
    "assets/shaders/debug.frag"
  );
}
void PhysicsManager::tick(float dt)
{
  // reset grounded
  for (const auto& physics : m_physicsComponents) {
    if (physics->isDynamic())
      physics->setIsGrounded(false);
  }

  // gravity
  for (const auto& physics : m_physicsComponents) {
    if (physics->isDynamic()) {
      glm::vec2 velocity = physics->getVelocity();
      float g = (velocity.y < 0.0f) ? GRAVITY * FALL_GRAVITY_MULT : GRAVITY;
      velocity.y -= g * dt;
      physics->setVelocity(velocity);
    }
  }

  // collisions
  for (int i = 0; i < m_dynamicColliders.size(); i++) {
    for (int j = i + 1; j < m_dynamicColliders.size(); j++) {
      CollisionInfo info = checkCollision(*m_dynamicColliders[i], *m_dynamicColliders[j]);
      if (info.collision) {
        m_dynamicColliders[i]->fireOnHit(m_dynamicColliders[j]->getOwnerId());
        m_dynamicColliders[j]->fireOnHit(m_dynamicColliders[i]->getOwnerId());
        if (!m_dynamicColliders[i]->getIsTrigger() && !m_dynamicColliders[j]->getIsTrigger())
          resolveCollision(*m_dynamicColliders[i], *m_dynamicColliders[j], info);
      }
    }
  }
  for (const auto& aCollider : m_dynamicColliders) {
    for (const auto& bCollider : m_staticColliders) {
      CollisionInfo info = checkCollision(*aCollider, *bCollider);
      if (info.collision) {
        aCollider->fireOnHit(bCollider->getOwnerId());
        bCollider->fireOnHit(aCollider->getOwnerId());
        if (!aCollider->getIsTrigger() && !bCollider->getIsTrigger())
          resolveCollision(*aCollider, *bCollider, info);
      }
    }
  }

  // friction 
  for (const auto& physics : m_physicsComponents) {
    if (physics->isDynamic() && physics->getIsGrounded() && !physics->getHasMovementInput()) {
      glm::vec2 velocity = physics->getVelocity();
      velocity.x *= (1.0f - physics->getFriction() * dt * 60.0f * dt);
      if (std::abs(velocity.x) < 0.05f)
        velocity.x = 0.0f;
      physics->setVelocity(velocity);
    }
  }

  for (const auto& collider : m_dynamicColliders) {
    collider->getOwner()->applyForces(dt);
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
    glm::vec2 aPos = aCollider.getColliderCenter();
    glm::vec2 bPos = bCollider.getColliderCenter();
    glm::vec2 aSize = aCollider.getColliderSize();
    glm::vec2 bSize = bCollider.getColliderSize();

    glm::vec2 aMin = aPos - aSize * 0.5f;
    glm::vec2 aMax = aPos + aSize * 0.5f;
    glm::vec2 bMin = bPos - bSize * 0.5f;
    glm::vec2 bMax = bPos + bSize * 0.5f;

    bool collisionX = aMax.x >= bMin.x && bMax.x >= aMin.x;
    bool collisionY = aMax.y >= bMin.y && bMax.y >= aMin.y;

    if (collisionX && collisionY) {
      return CollisionInfo(aCollider.getOwner(), bCollider.getOwner(), "box/box");
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

  // maybe use collider metrics instead
  glm::vec2 aPos = aCollider.getColliderCenter();
  glm::vec2 bPos = bCollider.getColliderCenter();

  glm::vec2 aHalf = aCollider.getColliderSize() * 0.5f;
  glm::vec2 bHalf = bCollider.getColliderSize() * 0.5f;

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

  //std::cout << "px=" << px << " py=" << py
  //  << " aHalf=" << aHalf.y
  //  << " bHalf=" << bHalf.y
  //  << std::endl;
  if (resolveX) {
    float sx = (dx < 0 ? -px : px);
    aPos.x += sx * aMove;
    bPos.x -= sx * bMove;

    if (aPhysics && bPhysics && aPhysics->isDynamic() && bPhysics->isDynamic()) {
      float aMass = aPhysics->getMass();
      float bMass = bPhysics->getMass();
      float totalMass = aMass + bMass;

      glm::vec2 aVel = aPhysics->getVelocity();
      glm::vec2 bVel = bPhysics->getVelocity();

      float newAx = (aVel.x * (aMass - bMass) + 2 * bMass * bVel.x) / totalMass;
      float newBx = (bVel.x * (bMass - aMass) + 2 * aMass * aVel.x) / totalMass;

      aPhysics->setVelocity(glm::vec2{ newAx, aVel.y });
      bPhysics->setVelocity(glm::vec2{ newBx, bVel.y });
    }
    else {
      // zero out x if one is static and the other is dynamic
      if (aPhysics && aPhysics->isDynamic())
        aPhysics->setVelocity(glm::vec2{ 0.0f, aPhysics->getVelocity().y });
      if (bPhysics && bPhysics->isDynamic())
        bPhysics->setVelocity(glm::vec2{ 0.0f, bPhysics->getVelocity().y });
    }
  }
  else {
    float sy = (dy < 0 ? -py : py);
    aPos.y += sy * aMove;
    bPos.y -= sy * bMove;

    if (aPhysics && aPhysics->isDynamic()) {
      glm::vec2 velocity = aPhysics->getVelocity();
      if (sy > 0.0f && velocity.y <= 0.0f) {
        velocity.y = 0.0f;
        aPhysics->setIsGrounded(true);

        // apply friction from surface
        float friction = bPhysics ? bPhysics->getFriction() : 0.5f;
        velocity.x *= (1.0f - friction);
      }
      else if (sy < 0.0f && velocity.y >= 0.0f) {
        velocity.y = 0.0f;
      }
      aPhysics->setVelocity(velocity);
    }
    if (bPhysics && bPhysics->isDynamic()) {
      glm::vec2 velocity = bPhysics->getVelocity();
      if (sy < 0.0f && velocity.y >= 0.0f) {
        velocity.y = 0.0f;
        bPhysics->setIsGrounded(true);

        float friction = aPhysics ? aPhysics->getFriction() : 0.5f;
        velocity.x *= (1.0f - friction);
      }
      else if (sy > 0.0f && velocity.y <= 0.0f) {
        velocity.y = 0.0f;
      }
      bPhysics->setVelocity(velocity);
    }
  }

  aTransform->setTranslation(aPos - aCollider.getColliderOffset());
  bTransform->setTranslation(bPos - bCollider.getColliderOffset());
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
void PhysicsManager::setColliderSize(uint64_t owner, const glm::vec2& size)
{
  Collision* c = getCollisionComponent(owner);
  if (c) {
    c->setColliderSize(size);
  }
}

void PhysicsManager::setColliderOffset(uint64_t owner, const glm::vec2& offset)
{
  Collision* c = getCollisionComponent(owner);
  if (c) c->setColliderOffset(offset);
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
  if (!m_drawDebug)
    return;

  m_debugShader->activate();
  m_debugVAO->bind();

  float zoom = Engine::get().getGraphicsManager().getCamera()->getZoom();
  float hw = 8.0f / zoom;
  float hh = 4.5f / zoom;
  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection = glm::ortho(-hw, hw, -hh, hh, -1.0f, 1.0f);
  m_debugShader->setMat4("view", view);
  m_debugShader->setMat4("projection", projection);

  auto drawColliders = [&](const auto& colliders, const glm::vec4& color)
    {
      m_debugShader->setVec4("color", color);
      for (const auto& collider : colliders)
      {
        GameObject* owner = collider->getOwner();
        if (!owner) continue;

        glm::vec2 pos = collider->getColliderCenter();
        glm::vec2 scale = collider->getColliderSize();

        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(pos, 0.0f));
        model = glm::scale(model, glm::vec3(scale, 1.0f));
        m_debugShader->setMat4("model", model);

        glDrawArrays(GL_LINE_LOOP, 0, 4);
      }
    };

  drawColliders(m_dynamicColliders, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)); // green — dynamic
  drawColliders(m_staticColliders, glm::vec4(1.0f, 0.3f, 0.0f, 1.0f)); // orange — static

  m_debugVAO->unbind();
}
void PhysicsManager::unregisterObject(uint64_t id)
{
  auto removeById = [id](auto& vec) {
    vec.erase(
      std::remove_if(vec.begin(), vec.end(),
        [id](const auto& c) { 
          return c->getOwnerId() == id; 
        }),
      vec.end()
    );
  };
  removeById(m_dynamicColliders);
  removeById(m_staticColliders);
  removeById(m_physicsComponents);
}

void PhysicsManager::shutdown()
{

}