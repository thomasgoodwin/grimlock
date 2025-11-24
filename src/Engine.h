#ifndef ENGINE_H
#define ENGINE_H
#include <chrono>
#include <ctime> 
#include <vector>
#include <memory>
#include <array>

class GraphicsManager;
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
  void addGameObject(std::string& name);
  void testCase1();
  void killEngine();

private:
	Engine();
	static Engine _instance;
	void tick(float dt);
	void render();
  std::unique_ptr<GraphicsManager> m_graphicsManager;
	float m_deltaTime;
	bool m_gameIsRunning = true;
	std::chrono::system_clock::time_point m_currentTime;
  std::vector<std::unique_ptr<GameObject>> m_gameObjects;
};

#endif