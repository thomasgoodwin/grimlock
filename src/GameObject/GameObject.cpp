#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "GameObject.h"
#include "../Graphics/GraphicsManager.h"
#include "../Engine.h"

GameObject::GameObject()
{
  m_texture = std::make_shared<Texture>("assets/textures/cool.png");
  m_transform = std::make_shared<Transform>();
  m_shader = Engine::get().getGraphicsManager().addShader("assets/shaders/simple.vert", "assets/shaders/simple.frag");
  m_vertices = {
    // positions        // texcoords
   -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,  // bottom-left
    0.5f, -0.5f, 0.0f,  1.0f, 0.0f,  // bottom-right
    0.5f,  0.5f, 0.0f,  1.0f, 1.0f,  // top-right
   -0.5f,  0.5f, 0.0f,  0.0f, 1.0f   // top-left
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
  m_vao->linkAttribute(*m_vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);                 // position
  m_vao->linkAttribute(*m_vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float))); // color
  m_vao->linkAttribute(*m_vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float))); // texcoords
  m_vao->unbind();
}

void GameObject::initialize()
{
  LinkGraphics();
}

void GameObject::tick(float dt)
{
  float currentRotation = m_transform->getRotation();
  m_transform->setRotation(currentRotation + dt);
}

void GameObject::render()
{
  m_shader->activate();
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