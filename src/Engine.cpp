#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Graphics/GraphicsManager.h"
#include "Physics/PhysicsManager.h"
#include "GameObject/GameObject.h"
#include "Engine.h"
#include "Util.h"

Engine::Engine() : m_graphicsManager(std::make_unique<GraphicsManager>()), m_physicsManager(std::make_unique<PhysicsManager>())
{

}

void Engine::initialize()
{
  m_graphicsManager->initialize();
  m_physicsManager->initialize();
  m_currentTime = std::chrono::system_clock::now();
  for (auto& [id, gameObject] : m_gameObjects) {
    gameObject->initialize();
  }
}

Engine::~Engine()
{
}

bool Engine::gameIsRunning()
{
  return m_gameIsRunning;
}

void Engine::gameLoop()
{
  glfwPollEvents();
  auto newCurrentTime = std::chrono::system_clock::now();
  std::chrono::duration<float> elapsedTime = (newCurrentTime - m_currentTime);
  m_currentTime = newCurrentTime;
  float dt = elapsedTime.count();
  m_deltaTime = dt;
  tick(dt);
  render();
}

float Engine::getDeltaTime()
{
  return m_deltaTime;
}

void Engine::shutdown()
{
  for (auto& [id, gameObject] : m_gameObjects) {
    gameObject->shutdown();
  }
  m_graphicsManager->shutdown();
  m_physicsManager->shutdown();
}

void Engine::stopGame()
{
  m_gameIsRunning = false;
}

void Engine::tick(float dt)
{
  for (auto& [id, gameObject] : m_gameObjects) {
    if (!gameObject->isDisabled()) {
      gameObject->tick(dt);
    }
  }
  m_graphicsManager->tick(dt);
  m_physicsManager->tick(dt);
}

void Engine::render()
{
  for (auto& [id, gameObject] : m_gameObjects) {
    if (!gameObject->isDisabled()) {
      gameObject->render();
    }
  }
  m_graphicsManager->prerender();
  m_graphicsManager->render();
  m_physicsManager->render();
  m_graphicsManager->postrender();
}

GraphicsManager& Engine::getGraphicsManager()
{
  return *reinterpret_cast<GraphicsManager*>(Engine::get().m_graphicsManager.get());
}

Engine& Engine::get()
{
  static Engine _instance;
  return _instance;
}

uint64_t Engine::addGameObject(std::string& name)
{
  uint64_t objectId = generate_uuid();
  m_gameObjects[objectId] = std::make_shared<GameObject>(name, objectId);
  return objectId;
}

std::weak_ptr<GameObject> Engine::getGameObjectById(uint64_t id)
{
  try {
    m_gameObjects.at(id);
  }
  catch (const std::out_of_range& e) {
    std::cout << "Game object does not exist: " << id << std::endl;
    return std::weak_ptr<GameObject>();
  }
  return m_gameObjects[id];
}

void Engine::killEngine() {
  m_gameIsRunning = false;
}

void Engine::printGameObjects() const
{
  std::cout << "<Print Game Objects>" << std::endl;
  for (const auto& [key, value] : m_gameObjects) {
    std::cout << "Key: " << key << ", Value: " << value->getObjectName() << std::endl;
  }
}

// test cases
void Engine::testCase1()
{
  uint64_t mainBoxId = addGameObject(std::string("main box"));
  m_physicsManager->registerCollisionComponent(mainBoxId, "box");
}