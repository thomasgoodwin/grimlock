#ifndef MELEE_ENEMY_H
#define MELEE_ENEMY_H
#include <memory>
#include <string>
#include "GameObject/GameObject.h"
#include "AI/BehaviorComponent.h"
#include "AI/MeleeBehavior.h"

class MeleeEnemy : public GameObject
{
public:
  MeleeEnemy(uint64_t id, const std::string& name);

  void initialize() override;
  void tick(float dt) override;
  void shutdown() override;

  const std::string getTypename() const override { return "MeleeEnemy"; }

  void setBehavior(std::unique_ptr<BehaviorComponent> behavior);
  BehaviorComponent* getBehavior() const { return m_behavior.get(); }

private:
  std::unique_ptr<BehaviorComponent> m_behavior;
};

#endif
