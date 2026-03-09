#include "EventManager.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

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

void EventManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
  else {
    std::cout << key << scancode << action << mods << std::endl;
  }
}

