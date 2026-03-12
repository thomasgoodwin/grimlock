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

GameObject::GameObject(uint64_t id, const std::string& name, const std::string& texturePath) : m_objectName(name), m_id(id)
{
  m_texture = std::make_shared<Texture>(texturePath);
  m_transform = std::make_shared<Transform>();

  // Size the quad to match the texture's pixel dimensions in world units
  if (m_texture->getWidth() > 0 && m_texture->getHeight() > 0)
  {
    float w = m_texture->getWidth() / PIXELS_PER_UNIT;
    float h = m_texture->getHeight() / PIXELS_PER_UNIT;
    m_transform->setScale(glm::vec2(w, h));
  }

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
  LinkGraphics();
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
  if (m_animatedSprite)
  {
    m_animatedSprite->tick(dt);
  }
}

void GameObject::render()
{
  m_vao->bind();
  m_shader->activate();
  float zoom = Engine::get().getGraphicsManager().getCamera()->getZoom();
  float hw = 8.0f / zoom;
  float hh = 4.5f / zoom;
  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection = glm::ortho(-hw, hw, -hh, hh, -1.0f, 1.0f);
  m_shader->setMat4("model", m_transform->getMatrix());
  m_shader->setMat4("view", view);
  m_shader->setMat4("projection", projection);
  m_texture->bind(0);
  m_shader->setUniform1i("tex0", 0);

  if (m_animatedSprite && m_animatedSprite->hasActiveAnimation())
  {
    m_shader->setVec2("uvOffset", m_animatedSprite->getUVOffset());
    m_shader->setVec2("uvScale", m_animatedSprite->getUVScale());
  }
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

void GameObject::attachAnimatedSprite(int sheetCols, int sheetRows)
{
  m_animatedSprite = std::make_shared<AnimatedSpriteComponent>(sheetCols, sheetRows);

  if (m_texture->getWidth() > 0 && m_texture->getHeight() > 0)
  {
    float w = (m_texture->getWidth() / static_cast<float>(sheetCols)) / PIXELS_PER_UNIT;
    float h = (m_texture->getHeight() / static_cast<float>(sheetRows)) / PIXELS_PER_UNIT;
    m_transform->setScale(glm::vec2(w, h));
  }

  m_shader = Engine::get().getGraphicsManager().addShader(
    "assets/shaders/spritesheet.vert",
    "assets/shaders/spritesheet.frag"
  );
}

std::shared_ptr<AnimatedSpriteComponent> GameObject::getAnimatedSprite()
{
  return m_animatedSprite;
}

void GameObject::applyForces(float dt)
{
  // apply gravity and momentum
  glm::vec2 velocity = Engine::get().getPhysicsManager().getPhysicsComponent(m_id)->getVelocity();
  glm::vec2 position = m_transform->getTranslation();
  position += velocity * dt;
  m_transform->setTranslation(position);
}

uint64_t GameObject::getId() const
{
  return m_id;
}
