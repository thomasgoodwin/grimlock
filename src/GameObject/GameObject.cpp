#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "GameObject.h"
#include "../Graphics/GraphicsManager.h"
#include "../Engine.h"

GameObject::GameObject(std::string& name)
{
  m_objectName = name;
  m_texture = std::make_shared<Texture>("assets/textures/cool.png");
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
  std::cout << m_objectName << " constructed" << std::endl;
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
  LinkGraphics();
  std::cout << m_objectName << " initialized" << std::endl;
}

void GameObject::tick(float dt)
{
  //float currentRotation = m_transform->getRotation();
  //m_transform->setRotation(currentRotation + (dt * 20));
  m_transform->debug();
}

void GameObject::render()
{
  auto camera = Engine::get().getGraphicsManager().getCamera();
  m_shader->activate();

  // temp 
  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection = glm::mat4(1.0f);
  m_shader->setMat4("model", m_transform->getMatrix());
  m_shader->setMat4("view", view);
  m_shader->setMat4("projection", projection);
  m_texture->bind(0);
  m_shader->setUniform1i("tex0", 0);
}

void GameObject::shutdown()
{
  std::cout << m_objectName << " shutdown" << std::endl;
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