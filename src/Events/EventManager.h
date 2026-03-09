#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H
#include <queue>
#include <vector>
#include <functional>
#include <unordered_map>

struct GLFWwindow;

class EventManager
{
public:
  using Event = std::function<void()>;
  EventManager();
  ~EventManager();
  static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
  void enqueue(Event event);

  // Register a callback for a specific GLFW key + action (GLFW_PRESS / GLFW_RELEASE)
  void bindKey(int glfwKey, int glfwAction, Event callback);

  // Called from keyCallback to dispatch into the bound callbacks
  void dispatchKeyEvent(int key, int action);

  void initialize();
  void tick(float dt);
  void render();
  void shutdown();
private:
  std::queue<Event> m_events;
  std::unordered_map<int, std::vector<Event>> m_keyPressBindings;
  std::unordered_map<int, std::vector<Event>> m_keyReleaseBindings;
};

#endif