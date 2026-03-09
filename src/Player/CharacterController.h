#ifndef CHARACTER_CONTROLLER_H
#define CHARACTER_CONTROLLER_H
#include <cstdint>

class CharacterController
{
public:
  CharacterController(uint64_t ownerId);

  void initialize();
  void tick(float dt);
  void shutdown();

private:
  // Re-evaluates movement + animation + flip from current held-key state.
  // Called on every A/D press and release so both keys are always considered.
  void applyHorizontal();

  uint64_t m_ownerId;
  bool m_leftHeld  = false;
  bool m_rightHeld = false;
};

#endif
