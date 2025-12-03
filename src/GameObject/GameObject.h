#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include <string>
#include <vector>
#include <memory>

#include "Transform.h"
#include "Graphics/Texture.h"
#include "Graphics/Shader.h"
#include "Graphics/VBO.h"
#include "Graphics/VAO.h"
#include "Graphics/EBO.h"

class GameObject
{
public:
  GameObject();
  GameObject(const std::string& name, uint64_t id, const std::string& texturePath);

  virtual void initialize();
  virtual void tick(float dt);
  virtual void render();
  virtual void shutdown();

  void LinkGraphics();

  virtual void setObjectName(const std::string& name) { m_objectName = name; };
  virtual const std::string& getObjectName() const { return m_objectName; };

  virtual const std::string getTypename(void) const { return ""; };

  void markObjectForDestruction();
  bool isObjectMarkedForDestruction() const { return m_isMarkedForDestruction; };

  void disable();
  bool isDisabled();

  std::shared_ptr<Texture> getTexture();
  std::shared_ptr<Transform> getTransform();

  void applyForces(float dt);

private:
  bool m_isMarkedForDestruction = false;
  bool m_isDisabled = false;
  std::string m_objectName = "default object name";

  // graphics
  std::shared_ptr<Texture> m_texture;
  std::shared_ptr<Transform> m_transform;
  std::shared_ptr<Shader> m_shader;
  std::shared_ptr<VAO> m_vao;
  std::shared_ptr<VBO> m_vbo;
  std::shared_ptr<EBO> m_ebo;
  std::vector<float> m_vertices;
  std::vector<unsigned int> m_indices;
  uint64_t m_id;
};

#endif