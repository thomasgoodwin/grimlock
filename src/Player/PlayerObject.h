#ifndef PLAYER_OBJECT_H
#define PLAYER_OBJECT_H
#include <string>
#include "GameObject/GameObject.h"
#include "CharacterController.h"

class PlayerObject : public GameObject
{
public:
  PlayerObject(uint64_t id, const std::string& name);

  void initialize() override;
  void tick(float dt) override;
  void shutdown() override;
  void takeDamage(float amount) override;

  const std::string getTypename() const override { return "PlayerObject"; };

private:
  CharacterController m_controller;
  float m_damageTintTimer = 0.0f;
  static constexpr float DAMAGE_TINT_DURATION = 1.3f;
};

#endif
