#ifndef ANIMATED_SPRITE_COMPONENT_H
#define ANIMATED_SPRITE_COMPONENT_H
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include "AnimationClip.h"

enum class AnimationPlayMode
{
  Cycling,
  Event
};

class AnimatedSpriteComponent
{
public:
  AnimatedSpriteComponent(int sheetCols, int sheetRows);

  void addClip(const AnimationClip& clip);

  void setCyclingAnimation(const std::string& name);

  void triggerAnimation(const std::string& name);

  void tick(float dt);

  glm::vec2 getUVOffset() const;
  glm::vec2 getUVScale() const;

  bool hasActiveAnimation() const;

  void setFlipX(bool flip);

private:
  int m_sheetCols;
  int m_sheetRows;

  std::unordered_map<std::string, AnimationClip> m_clips;

  AnimationPlayMode m_mode = AnimationPlayMode::Cycling;

  std::string m_currentClipName;
  std::string m_cyclingClipName;

  int m_currentFrame = 0;
  float m_elapsed    = 0.0f;
  bool m_flipX       = false;
};

#endif
