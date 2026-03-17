#include "AnimatedSpriteComponent.h"

AnimatedSpriteComponent::AnimatedSpriteComponent(int sheetCols, int sheetRows)
  : m_sheetCols(sheetCols), m_sheetRows(sheetRows)
{
}

void AnimatedSpriteComponent::addClip(const AnimationClip& clip)
{
  m_clips[clip.name] = clip;
}

void AnimatedSpriteComponent::setCyclingAnimation(const std::string& name)
{
  if (m_clips.find(name) == m_clips.end())
    return;

  m_cyclingClipName = name;

  if (m_mode != AnimationPlayMode::Event && m_currentClipName != name)
  {
    m_currentClipName = name;
    m_currentFrame = m_clips.at(name).startFrame;
    m_elapsed = 0.0f;
    m_mode = AnimationPlayMode::Cycling;
  }
}

void AnimatedSpriteComponent::triggerAnimation(const std::string& name)
{
  if (m_clips.find(name) == m_clips.end()) 
    return;

  m_currentClipName = name;
  m_currentFrame = m_clips.at(name).startFrame;
  m_elapsed = 0.0f;
  m_mode = AnimationPlayMode::Event;
}

void AnimatedSpriteComponent::tick(float dt)
{
  if (m_currentClipName.empty()) return;

  const AnimationClip& clip = m_clips.at(m_currentClipName);
  float frameDuration = 1.0f / clip.fps;

  m_elapsed += dt;

  while (m_elapsed >= frameDuration)
  {
    m_elapsed -= frameDuration;
    m_currentFrame++;

    if (m_currentFrame > clip.endFrame)
    {
      bool shouldLoop = (m_mode == AnimationPlayMode::Cycling) || clip.loop;

      if (shouldLoop)
      {
        m_currentFrame = clip.startFrame;
      }
      else
      {
        m_currentFrame = clip.endFrame;
        if (!m_cyclingClipName.empty())
        {
          m_currentClipName = m_cyclingClipName;
          m_currentFrame = m_clips.at(m_cyclingClipName).startFrame;
          m_elapsed = 0.0f;
          m_mode = AnimationPlayMode::Cycling;
        }
        break;
      }
    }
  }
}

glm::vec2 AnimatedSpriteComponent::getUVOffset() const
{
  if (m_currentClipName.empty())
    return glm::vec2(0.0f, 0.0f);

  const AnimationClip& clip = m_clips.at(m_currentClipName);

  float x = static_cast<float>(m_currentFrame) / static_cast<float>(m_sheetCols) + clip.offsetX;
  float y = static_cast<float>(m_sheetRows - 1 - clip.row) / static_cast<float>(m_sheetRows) - clip.offsetY;

  if (m_flipX)
    x += 1.0f / static_cast<float>(m_sheetCols);

  return glm::vec2(x, y);
}

glm::vec2 AnimatedSpriteComponent::getUVScale() const
{
  float scaleX = 1.0f / static_cast<float>(m_sheetCols);
  if (m_flipX) scaleX = -scaleX;

  return glm::vec2(scaleX, 1.0f / static_cast<float>(m_sheetRows));
}

bool AnimatedSpriteComponent::hasActiveAnimation() const
{
  return !m_currentClipName.empty();
}

bool AnimatedSpriteComponent::isEventMode() const
{
  return m_mode == AnimationPlayMode::Event;
}

void AnimatedSpriteComponent::setFlipX(bool flip)
{
  m_flipX = flip;
}
