#include "EventManager.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Engine.h"

EventManager::EventManager()
{

}
EventManager::~EventManager()
{

}
void EventManager::initialize()
{

}
void EventManager::tick(float dt)
{
  while (!m_events.empty()) {
    auto event = std::move(m_events.front());
    m_events.pop();
    event();
  }
}
/* sample usage for later
  int entityId = 42;
  eventManager.enqueue([objectId]() {
      damageObject(objectId, 10);
  });
*/
void EventManager::render()
{

}
void EventManager::shutdown()
{

}

void EventManager::enqueue(Event event) {
  m_events.push(std::move(event));
}

void EventManager::bindKey(int glfwKey, int glfwAction, Event callback)
{
  if (glfwAction == GLFW_PRESS)
  {
    m_keyPressBindings[glfwKey].push_back(std::move(callback));
  }
  else if (glfwAction == GLFW_RELEASE)
  {
    m_keyReleaseBindings[glfwKey].push_back(std::move(callback));
  }
}

void EventManager::dispatchKeyEvent(int key, int action)
{
  if (action == GLFW_PRESS)
  {
    auto it = m_keyPressBindings.find(key);
    if (it != m_keyPressBindings.end())
    {
      for (auto& cb : it->second)
      {
        enqueue(cb);
      }
    }
  }
  else if (action == GLFW_RELEASE)
  {
    auto it = m_keyReleaseBindings.find(key);
    if (it != m_keyReleaseBindings.end())
    {
      for (auto& cb : it->second)
      {
        enqueue(cb);
      }
    }
  }
}

void EventManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
    return;
  }
  Engine::get().getEventManager().dispatchKeyEvent(key, action);
}

