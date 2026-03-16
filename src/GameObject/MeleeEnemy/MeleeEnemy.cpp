#include "MeleeEnemy.h"
#include "Graphics/AnimationClip.h"
#include "Engine.h"

static const char* MELEE_ENEMY_SPRITE = "assets/pixel-asset-pack/Enemies/Alien6.png";
static const int SHEET_COLS = 8;
static const int SHEET_ROWS = 6;

MeleeEnemy::MeleeEnemy(uint64_t id, const std::string& name)
  : GameObject(id, name, MELEE_ENEMY_SPRITE)
{
  attachAnimatedSprite(SHEET_COLS, SHEET_ROWS);

  auto anim = getAnimatedSprite();
  anim->addClip({ "idle", 0, 0, 3, 4.0f, true });
  anim->setCyclingAnimation("idle");

  attachHealth(100.0f);
  setIsHostile(true);
}

void MeleeEnemy::initialize()
{
  GameObject::initialize();
  if (m_behavior)
    m_behavior->initialize();
}

void MeleeEnemy::tick(float dt)
{
  GameObject::tick(dt);
  if (m_behavior)
    m_behavior->tick(dt);
}

void MeleeEnemy::shutdown()
{
  if (m_behavior)
    m_behavior->shutdown();
  GameObject::shutdown();
}

void MeleeEnemy::setBehavior(std::unique_ptr<BehaviorComponent> behavior)
{
  m_behavior = std::move(behavior);
}
