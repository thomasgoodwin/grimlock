#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

Camera::Camera(float screenWidth, float screenHeight)
  : m_width(screenWidth), m_height(screenHeight),
  m_position(0.0f), m_zoom(1.0f), m_rotation(0.0f)
{
}

void Camera::setPosition(const glm::vec2& pos)
{
  m_position = pos;
}
void Camera::setZoom(float zoom)
{
  m_zoom = zoom;
}
void Camera::setRotation(float rot)
{
  m_rotation = rot;
}

glm::vec2 Camera::getPosition() const
{
  return m_position;
}
float Camera::getZoom() const
{
  return m_zoom;
}
float Camera::getRotation() const
{
  return m_rotation;
}

glm::mat3 Camera::getViewMatrix() const
{
  glm::mat3 T = {
      {1, 0, -m_position.x},
      {0, 1, -m_position.y},
      {0, 0, 1}
  };

  float c = cos(-m_rotation);
  float s = sin(-m_rotation);

  glm::mat3 R = {
      {c, -s, 0},
      {s,  c, 0},
      {0,  0, 1}
  };

  glm::mat3 S = {
      {m_zoom, 0,       0},
      {0,      m_zoom,  0},
      {0,      0,       1}
  };

  return S * R * T;
}

glm::mat3 Camera::getProjectionMatrix() const
{
  float w = m_width;
  float h = m_height;

  return glm::mat3(
    2.0f / w, 0.0f, -1.0f,
    0.0f, -2.0f / h, 1.0f,
    0.0f, 0.0f, 1.0f
  );
}