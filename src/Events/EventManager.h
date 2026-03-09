#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H
#include <queue>
#include <functional>

struct GLFWwindow;

class Event {

};

class EventManager
{
public:
  using Event = std::function<void()>;
  EventManager();
  ~EventManager();
  static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
  void enqueue(Event event);
  void initialize();
  void tick(float dt);
  void render();
  void shutdown();
private:
  std::queue<Event> m_events;
};

#endif