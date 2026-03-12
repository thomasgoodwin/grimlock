#ifndef CHARACTER_CONTROLLER_H
#define CHARACTER_CONTROLLER_H
#include <cstdint>
#include <string>

class CharacterController
{
public:
  CharacterController(uint64_t ownerId);

  void initialize();
  void tick(float dt);
  void shutdown();

  const std::string& getDirection() const { return m_direction; };

private:
  // Re-evaluates movement + animation + flip from current held-key state.
  // Called on every A/D press and release so both keys are always considered.
  void onHorizontalInput();

  uint64_t m_ownerId;
  bool m_leftHeld = false;
  bool m_rightHeld = false;
  std::string m_direction = "right";
  float m_coyoteTimer = 0.0f;
  static constexpr float COYOTE_TIME = 0.1f;
};

#endif
