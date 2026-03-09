#ifndef PLAYER_OBJECT_H
#define PLAYER_OBJECT_H
#include <string>
#include "GameObject/GameObject.h"
#include "CharacterController.h"

class PlayerObject : public GameObject
{
public:
  PlayerObject(const std::string& name, uint64_t id);

  void initialize() override;
  void tick(float dt) override;
  void shutdown() override;

  const std::string getTypename() const override { return "PlayerObject"; };

private:
  CharacterController m_controller;
};

#endif
