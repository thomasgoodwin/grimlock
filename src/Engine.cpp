#include <iostream>
#include "Graphics/GraphicsManager.h"
#include "Engine.h"

Engine::Engine() : m_graphicsManager(std::make_unique<GraphicsManager>())
{

}

void Engine::initialize()
{
  m_graphicsManager->initialize();
  m_currentTime = std::chrono::system_clock::now();
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
  m_graphicsManager->shutdown();
}

void Engine::stopGame()
{
  m_gameIsRunning = false;
}

void Engine::tick(float dt)
{
  m_graphicsManager->tick(dt);
}

void Engine::render()
{
  m_graphicsManager->render();
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