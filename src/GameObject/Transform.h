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
  void setRotation(float rotationDegrees);
  void setTranslation(glm::vec2 translation);

  glm::mat4 getMatrix();
  void debug();
private:
  glm::mat4 calculateMatrix();

  glm::vec2 m_scale;
  float m_rotation; // degrees
  glm::vec2 m_translation;

  glm::mat4 m_matrix;
};

#endif