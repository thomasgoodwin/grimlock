#include "Transform.h"
#include <math.h>
#define PI 3.14159265

Transform::Transform()
{
  m_rotation = 0.0f;
  m_scale = glm::vec2(1.0f, 1.0f);
  m_translation = glm::vec2(0.0f, 0.0f);
}

Transform::~Transform()
{

}

glm::vec2 Transform::getScale()
{
  return m_scale;
}

float Transform::getRotation()
{
  return m_rotation;
}

glm::vec2 Transform::getTranslation()
{
  return m_translation;
}

void Transform::setScale(glm::vec2 scale)
{
  m_scale = scale;
}

void Transform::setRotation(float rotation)
{
  m_rotation = rotation;
}

void Transform::setTranslation(glm::vec2 translation)
{
  m_translation = translation;
}

glm::mat3 Transform::calculateMatrix()
{
  glm::mat3 scale = {
    m_scale[0], 0, 0,
    0, m_scale[1], 0,
    0,0,1
  };

  float rotationRadians = (m_rotation * PI / 180); 
  glm::mat3 rotation = {
    cos(rotationRadians), -sin(rotationRadians), 0,
    sin(rotationRadians), cos(rotationRadians), 0,
    0,0,1
  };
  
  glm::mat3 translation = {
    1, 0, m_translation[0],
    0, 1, m_translation[1],
    0, 0, 1
  };

  glm::mat3 result =  scale * rotation * translation;
  return result;
}