#ifndef ENGINE_H
#define ENGINE_H
#include <chrono>
#include <ctime> 
#include <memory>
#include <array>
#include <unordered_map>
#include <queue>
#include "Util.h"

class GraphicsManager;
class PhysicsManager;
class GameObject;
class EventManager;
class AudioManager;

class Engine
{
public:
  ~Engine();
  void initialize();
  void gameLoop();
  void shutdown();
  void stopGame();
  bool gameIsRunning();
  float getDeltaTime();
  static Engine& get();
  static GraphicsManager& getGraphicsManager();
  static PhysicsManager& getPhysicsManager();
  static EventManager& getEventManager();
  static AudioManager& getAudioManager();
  std::weak_ptr<GameObject> getGameObjectById(uint64_t id);
  std::weak_ptr<GameObject> getPlayerObject();
  template <typename T, typename... Args>
  uint64_t addGameObject(Args&&... args)
  {
    static_assert(std::is_base_of<GameObject, T>::value, "T must derive from GameObject");
    uint64_t objectId = generate_uuid();
    m_gameObjects[objectId] = std::make_shared<T>(objectId, std::forward<Args>(args)...);
    return objectId;
  }
  uint64_t addPlayerObject(const std::string& name);
  void testCase1();
  void killEngine();
  void printGameObjects() const;
  void destroyObject(uint64_t id);
private:
  Engine();
  static Engine _instance;
  void tick(float dt);
  void render();
  std::unique_ptr<GraphicsManager> m_graphicsManager;
  std::unique_ptr<PhysicsManager> m_physicsManager;
  std::unique_ptr<EventManager> m_eventManager;
  std::unique_ptr<AudioManager> m_audioManager;
  float m_deltaTime;
  bool m_gameIsRunning = true;
  std::chrono::system_clock::time_point m_currentTime;
  std::unordered_map<uint64_t, std::shared_ptr<GameObject>> m_gameObjects;
  std::queue<uint64_t> m_destroyQueue;
};

#endif