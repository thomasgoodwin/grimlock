#include "PlayerObject.h"
#include "Graphics/AnimationClip.h"


// test
static const char* ROBOT_SPRITE = "assets/pixel-asset-pack/Main_Characters/Char_Robot.png";
static const int   SHEET_COLS   = 8;
static const int   SHEET_ROWS   = 6;

PlayerObject::PlayerObject(const std::string& name, uint64_t id)
  : GameObject(name, id, ROBOT_SPRITE), m_controller(id)
{
  attachAnimatedSprite(SHEET_COLS, SHEET_ROWS);

  auto anim = getAnimatedSprite();
  anim->addClip({ "idle", 2, 0, 2,  6.0f,  true });
  anim->addClip({ "run",  1, 0, 7,  12.0f, true });
  anim->addClip({ "jump", 3, 0, 5,  10.0f, false });

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
