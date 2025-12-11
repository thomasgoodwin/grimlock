#ifndef ENGINE_H
#define ENGINE_H
#include <chrono>
#include <ctime> 
#include <memory>
#include <array>
#include <unordered_map>

class GraphicsManager;
class PhysicsManager;
class GameObject;

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
  std::weak_ptr<GameObject> getGameObjectById(uint64_t id);
  uint64_t addGameObject(const std::string& name, const std::string& texturePath = std::string("assets/textures/cool.png"));
  void testCase1();
  void killEngine();
  void printGameObjects() const;
private:
  Engine();
  static Engine _instance;
  void tick(float dt);
  void render();
  std::unique_ptr<GraphicsManager> m_graphicsManager;
  std::unique_ptr<PhysicsManager> m_physicsManager;
  float m_deltaTime;
  bool m_gameIsRunning = true;
  std::chrono::system_clock::time_point m_currentTime;
  std::unordered_map<uint64_t, std::shared_ptr<GameObject>> m_gameObjects;
};

#endif