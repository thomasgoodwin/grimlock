#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "PlayerObject.h"
#include "Graphics/AnimationClip.h"
#include "Events/EventManager.h"
#include "Engine.h"
#include <iostream>
#include "GameObject/Bullet/BulletObject.h"
#include "Physics/PhysicsManager.h"
#include "Physics/PhysicsComponent.h"

// test
static const char* ROBOT_SPRITE = "assets/pixel-asset-pack/Main_Characters/Char_Robot.png";
static const int SHEET_COLS = 8;
static const int SHEET_ROWS = 6;

PlayerObject::PlayerObject(uint64_t id, const std::string& name)
  : GameObject(id, name, ROBOT_SPRITE), m_controller(id)
{
  attachAnimatedSprite(SHEET_COLS, SHEET_ROWS);

  auto anim = getAnimatedSprite();
  anim->addClip({ "idle", 2, 0, 2,  6.0f,  true });
  anim->addClip({ "run",  1, 0, 7,  12.0f, true });
  anim->addClip({ "jump", 3, 0, 5,  10.0f, false });
  EventManager& em = Engine::get().getEventManager();
  em.bindKey(
    GLFW_KEY_SPACE,
    GLFW_PRESS,
    [this]() {
      auto& direction = m_controller.getDirection();
      glm::vec2 velocity = glm::vec2{ 0.0f, 0.0f };
      if (direction == "right") {
        velocity = glm::vec2{ 1.0f, 0.0f };
      }
      else if (direction == "left") {
        velocity = glm::vec2{ -1.0f, 0.0f };
      }
      std::cout << direction << std::endl;
      auto newBulletId = Engine::get().addGameObject<BulletObject>("bullet", velocity, 2.0f);
      Engine::get().getPhysicsManager().registerPhysicsComponent(newBulletId, BodyType::Dynamic);
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
}

void PlayerObject::shutdown()
{
  m_controller.shutdown();
  GameObject::shutdown();
}
