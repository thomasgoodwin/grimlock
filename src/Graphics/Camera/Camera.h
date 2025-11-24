#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera
{
public:
    Camera(int screenWidth, int screenHeight);

    void setPosition(const glm::vec2& pos);
    void setZoom(float zoom);
    void setRotation(float rotationRadians);

    glm::vec2 getPosition() const;
    float getZoom() const;
    float getRotation() const;

    glm::mat3 getViewMatrix() const;
    glm::mat3 getProjectionMatrix() const;

private:
    float m_width;
    float m_height;
    glm::vec2 m_position;
    float m_zoom;
    float m_rotation;
};

#endif