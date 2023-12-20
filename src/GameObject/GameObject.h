#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <SFML/Graphics.hpp>
#include "../Components/PhysicsComponent.h"
#include <string>
#include <vector>
#include <memory>

class Component;

class GameObject
{
public: // maybe remove virtual keywords on overridden (wacky on some compilers but I guess not ours)
  GameObject();

  virtual void initialize();
  virtual void shutdown();
  virtual void tick(float dt);
  virtual void render();

  virtual sf::RectangleShape& getShape() { return m_shape; };
  virtual const sf::RectangleShape& getShape() const { return m_shape; };

  virtual void physicsCallback(std::shared_ptr<GameObject>) = 0;

  void addComponent(const std::shared_ptr<Component>& component);
  void removeComponent(const std::shared_ptr<Component>& componentToRemove);

  virtual void setObjectName(const std::string& name) { m_objectName = name; };
  virtual const std::string& getObjectName() const  { return m_objectName; };

  const std::string getTypename(void) const  { return ""; };

  const std::vector<std::shared_ptr<Component>>& getAllComponents() const  { return m_components; };
  std::vector<std::shared_ptr<Component>>& getAllComponents()  { return m_components; };

  template <typename tComponentType>
  void getComponents(std::vector<std::shared_ptr<tComponentType>>& components) const
  {
    for (const std::shared_ptr<Component>& component : m_components)
    {
      std::shared_ptr<tComponentType> comp = std::dynamic_pointer_cast<tComponentType>(component);
      if (comp)
      {
        components.push_back(comp);
      }
    }
  }

  // only call if you know for sure that there is only one instance of a component attached
  template <typename tComponentType>
  std::shared_ptr<tComponentType> getComponentOfType()
  {
    for (const std::shared_ptr<Component>& component : m_components)
    {
      std::shared_ptr<tComponentType> comp = std::dynamic_pointer_cast<tComponentType>(component);
      if (comp)
      {
        return comp;
      }
    }
  }

  void markObjectForDestruction();
  bool isObjectMarkedForDestruction() const  { return m_isMarkedForDestruction; };

  void disable();
  bool isDisabled();

  std::weak_ptr<Component> lockComponent(const Component* component) const;

  template <typename tComponent>
  std::shared_ptr<tComponent> makeComponent()
  {
    std::shared_ptr<tComponent> component = std::make_shared<tComponent>();
    return component;
  }

  void setPos(sf::Vector2f newPos);

  void setSize(sf::Vector2f newSize);

  sf::Vector2f getPosition()
  {
    return m_shape.getPosition();
  }

  sf::Vector2f getSize()
  {
    return m_shape.getSize();
  }

  float getRight()
  {
    return m_shape.getPosition().x + m_shape.getSize().x;
  }

  float getLeft()
  {
    return m_shape.getPosition().x;
  }

  float getTop()
  {
    return m_shape.getPosition().y;
  }

  float getBottom()
  {
    return m_shape.getPosition().y + m_shape.getSize().y;
  }


private:
  bool m_isMarkedForDestruction = false;
  bool m_isDisabled = false;
   m_shape;
  std::string m_objectName = "default object name";
  std::vector<std::shared_ptr<Component>> m_components;
};

#endif