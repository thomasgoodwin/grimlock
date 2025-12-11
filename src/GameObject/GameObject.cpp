#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Graphics/GraphicsManager.h"
#include "Physics/PhysicsManager.h"
#include "Physics/Collision/Collision.h"
#include "Physics/PhysicsComponent.h"
#include "GameObject.h"
#include "Constants.h"
#include "Engine.h"

GameObject::GameObject()
{

}

GameObject::GameObject(const std::string& name, uint64_t id, const std::string& texturePath) : m_objectName(name), m_id(id)
{
  m_texture = std::make_shared<Texture>(texturePath);
  m_transform = std::make_shared<Transform>();
  m_shader = Engine::get().getGraphicsManager().addShader("assets/shaders/simple.vert", "assets/shaders/simple.frag");
  m_vertices = {
    // positions          // colors            // texcoords
   -0.5f, -0.5f, 0.0f,    1.0f, 0.0f, 0.0f,     0.0f, 0.0f,
    0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,     1.0f, 0.0f,
    0.5f,  0.5f, 0.0f,    0.0f, 0.0f, 1.0f,     1.0f, 1.0f,
   -0.5f,  0.5f, 0.0f,    1.0f, 1.0f, 1.0f,     0.0f, 1.0f
  };

  m_indices = {
    0, 1, 2,
    2, 3, 0
  };
  m_vao = std::make_shared<VAO>();
  m_vbo = std::make_shared<VBO>(m_vertices.data(), m_vertices.size() * sizeof(float));
  m_ebo = std::make_shared<EBO>(m_indices.data(), m_indices.size() * sizeof(unsigned int));
}

void GameObject::LinkGraphics()
{
  m_vao->bind();
  m_vbo->bind();
  m_ebo->bind();
  m_vao->linkAttribute(*m_vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);                   // position
  m_vao->linkAttribute(*m_vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float))); // color
  m_vao->linkAttribute(*m_vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float))); // texcoords
}

void GameObject::initialize()
{

}

void GameObject::tick(float dt)
{

}

void GameObject::render()
{
  LinkGraphics();
  m_shader->activate();
  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection = glm::ortho(-16.0f / 2.0f, 16.0f / 2.0f, -9.0f / 2.0f, 9.0f / 2.0f, -1.0f, 1.0f);
  m_shader->setMat4("model", m_transform->getMatrix());
  m_shader->setMat4("view", view);
  m_shader->setMat4("projection", projection);
  m_texture->bind(0);
  m_shader->setUniform1i("tex0", 0);
}

void GameObject::shutdown()
{
  m_shader->shutdown();
}

void GameObject::disable()
{
  m_isDisabled = true;
}

bool GameObject::isDisabled()
{
  return m_isDisabled;
}

void GameObject::markObjectForDestruction()
{
  m_isMarkedForDestruction = true;
}

std::shared_ptr<Texture> GameObject::getTexture()
{
  return m_texture;
}

std::shared_ptr<Transform> GameObject::getTransform()
{
  return m_transform;
}

void GameObject::applyForces(float dt)
{
  // apply gravity and momentum
  glm::vec2 velocity = Engine::get().getPhysicsManager().getPhysicsComponent(m_id)->getVelocity();
  glm::vec2 position = m_transform->getTranslation();
  position += velocity * dt;
  m_transform->setTranslation(position);
}