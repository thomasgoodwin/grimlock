#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Graphics/GraphicsManager.h"
#include "GameObject/GameObject.h"
#include "Engine.h"

Engine::Engine() : m_graphicsManager(std::make_unique<GraphicsManager>(1280, 720))
{

}

void Engine::initialize()
{
  m_graphicsManager->initialize();
  m_currentTime = std::chrono::system_clock::now();
  for (int i = 0; i < m_gameObjects.size(); i++) {
    m_gameObjects[i]->initialize();
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
  for (int i = 0; i < m_gameObjects.size(); i++) {
    m_gameObjects[i]->shutdown();
  }
  m_graphicsManager->shutdown();
}

void Engine::stopGame()
{
  m_gameIsRunning = false;
}

void Engine::tick(float dt)
{
  for (int i = 0; i < m_gameObjects.size(); i++) {
    if (!m_gameObjects[i]->isDisabled()) {
      m_gameObjects[i]->tick(dt);
    }
  }
  m_graphicsManager->tick(dt);
}

void Engine::render()
{
  for (int i = 0; i < m_gameObjects.size(); i++) {
    if (!m_gameObjects[i]->isDisabled()) {
      m_gameObjects[i]->render();
    }
  }
  m_graphicsManager->prerender();
  m_graphicsManager->render();
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

void Engine::addGameObject(std::string& name)
{
  m_gameObjects.push_back(std::make_unique<GameObject>(name));
}

void Engine::killEngine() {
  m_gameIsRunning = false;
}

// test cases
void Engine::testCase1()
{
  addGameObject(std::string("test"));
}