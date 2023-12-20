#ifndef TRANSFORM_H
#define TRANSFORM_H
#include <glm/glm.hpp>
#include <utility>

class Transform
{
public:
  Transform();
  ~Transform();
  glm::vec2 getScale();
  float getRotation();
  glm::vec2 getTranslation();
  void setScale(glm::vec2 scale);
  void setRotation(float rotation);
  void setTranslation(glm::vec2 translation);
  glm::mat3 calculateMatrix();
private:
  glm::mat3 m_transform;
  glm::vec2 m_scale;
  float m_rotation;
  glm::vec2 m_translation;
};

#endif