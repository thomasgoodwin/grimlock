#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "PlayerObject.h"
#include "Graphics/AnimationClip.h"
#include "Events/EventManager.h"
#include "Engine.h"
#include <iostream>
#include "GameObject/Bullet/BulletObject.h"
#include "Events/AudioEvent.h"
#include "Constants.h"
#include "GameObject/Transform.h"
#include "Physics/PhysicsManager.h"
#include "Physics/Collision/Collision.h"

// test
static const char* ROBOT_SPRITE = "assets/pixel-asset-pack/Main_Characters/Char_Robot.png";
static const int SHEET_COLS = 8;
static const int SHEET_ROWS = 6;

PlayerObject::PlayerObject(uint64_t id, const std::string& name)
  : GameObject(id, name, ROBOT_SPRITE), m_controller(id)
{
  attachAnimatedSprite(SHEET_COLS, SHEET_ROWS);
  attachHealth(100.0f);
  auto anim = getAnimatedSprite();
  anim->addClip({ "idle", 2, 0, 2,  6.0f,  true });
  anim->addClip({ "run",  1, 0, 7,  12.0f, true });
  anim->addClip({ "jump", 3, 0, 5,  10.0f, false });
  EventManager& em = Engine::get().getEventManager();
  em.bindKey(
    GLFW_KEY_SPACE,
    GLFW_PRESS,
    [this]()
    {
      EventManager& em = Engine::get().getEventManager();
      auto& direction = m_controller.getDirection();
      glm::vec2 velocity = glm::vec2{ 0.0f, 0.0f };
      if (direction == "right") {
        velocity = glm::vec2{ 1.0f, 0.0f };
      }
      else if (direction == "left") {
        velocity = glm::vec2{ -1.0f, 0.0f };
      }
      em.enqueue(AudioEvent("laser"));
      auto newBulletId = Engine::get().addGameObject<BulletObject>("bullet", velocity, BULLET_SPEED);
      if (auto bullet = Engine::get().getGameObjectById(newBulletId).lock())
      {
        glm::vec2 spawnPos = getTransform()->getTranslation() + velocity * BULLET_SPAWN_X_OFFSET;
        spawnPos += glm::vec2{ 0.0f, BULLET_SPAWN_Y_OFFSET };
        bullet->getTransform()->setTranslation(spawnPos);
        float scaleX = (velocity.x < 0) ? -0.5f : 0.5f;
        bullet->getTransform()->setScale(glm::vec2(scaleX, 0.5f));

        bool bulletIsHostile = bullet->isHostile();
        Engine::get().getPhysicsManager().registerCollisionComponent(newBulletId, "box");
        if (Collision* bulletCollider = Engine::get().getPhysicsManager().getCollisionComponent(newBulletId)) {
          bulletCollider->setIsTrigger(true);
          bulletCollider->setOnHit(
            [newBulletId, bulletIsHostile, bullet](uint64_t otherId)
            {
              if (auto other = Engine::get().getGameObjectById(otherId).lock()) {
                if (other->getHealth() && other->isHostile() != bulletIsHostile) {
                  other->takeDamage((std::static_pointer_cast<BulletObject>(bullet))->getDamage());
                  Engine::get().destroyObject(newBulletId);
                }
              }
            }
          );
        }
      }
    }
  );

  anim->setCyclingAnimation("idle");
}

void PlayerObject::initialize()
{
  GameObject::initialize();
  m_controller.initialize();
}

void PlayerObject::tick(float dt)
{
  GameObject::tick(dt);
  m_controller.tick(dt);
  glm::vec2 pos = getTransform()->getTranslation();
  if (pos.y < LEVEL_FLOOR) {
    getTransform()->setTranslation(glm::vec2(0.0f, 0.0f));
  }
}

void PlayerObject::shutdown()
{
  m_controller.shutdown();
  GameObject::shutdown();
}
