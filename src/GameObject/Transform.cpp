#include "Transform.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define PI 3.14159265f

Transform::Transform()
{
  m_rotation = 0.0f;
  m_scale = glm::vec2(1.0f, 1.0f);
  m_translation = glm::vec2(0.0f, 0.0f);

  m_matrix = calculateMatrix();
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
  m_matrix = calculateMatrix();
}

void Transform::setRotation(float rotationDegrees)
{
  m_rotation = rotationDegrees;
  m_matrix = calculateMatrix();
}

void Transform::setTranslation(glm::vec2 translation)
{
  m_translation = translation;
  m_matrix = calculateMatrix();
}

glm::mat4 Transform::calculateMatrix()
{
  glm::mat4 result = glm::mat4(1.0f);

  result = glm::translate(result, glm::vec3(m_translation, 0.0f));

  float radians = m_rotation * PI / 180.0f;
  result = glm::rotate(result, radians, glm::vec3(0.0f, 0.0f, 1.0f));

  result = glm::scale(result, glm::vec3(m_scale, 1.0f));

  return result;
}

glm::mat4 Transform::getMatrix()
{
  return m_matrix;
}

void Transform::debug()
{
  std::cout << "Translation: " << m_translation.x << ", " << m_translation.y << "\nRotation: " << m_rotation << "\n" << "Scale: " << m_scale.x << ", " << m_scale.y << "\n" << std::endl;
}