#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Graphics/GraphicsManager.h"
#include "Physics/PhysicsManager.h"
#include "Physics/PhysicsComponent.h"
#include "GameObject/GameObject.h"
#include "Player/PlayerObject.h"
#include "Events/EventManager.h"
#include "Audio/AudioManager.h"
#include "Engine.h"

Engine::Engine() :
  m_graphicsManager(std::make_unique<GraphicsManager>()),
  m_physicsManager(std::make_unique<PhysicsManager>()),
  m_eventManager(std::make_unique<EventManager>()),
  m_audioManager(std::make_unique<AudioManager>())
{

}

void Engine::initialize()
{
  m_graphicsManager->initialize();
  m_physicsManager->initialize();
  m_audioManager->initialize();
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
  m_audioManager->shutdown();
}

void Engine::stopGame()
{
  m_gameIsRunning = false;
}

void Engine::tick(float dt)
{
  while (!m_destroyQueue.empty()) {
    uint64_t id = m_destroyQueue.front();
    m_destroyQueue.pop();
    auto& gameObject = m_gameObjects.at(id);
    gameObject->shutdown();
    m_gameObjects.erase(id);
  }
  for (auto& [id, gameObject] : m_gameObjects) {
    if (!gameObject->isDisabled()) {
      gameObject->tick(dt);
    }
  }
  m_eventManager->tick(dt);
  m_graphicsManager->tick(dt);
  m_physicsManager->tick(dt);
  m_audioManager->tick(dt);
}

void Engine::render()
{
  m_graphicsManager->prerender();
  for (auto& [id, gameObject] : m_gameObjects) {
    if (!gameObject->isDisabled()) {
      gameObject->render();
      m_graphicsManager->render();
    }
  }
  m_physicsManager->render();
  m_audioManager->render();
  m_graphicsManager->postrender();
}

GraphicsManager& Engine::getGraphicsManager()
{
  return *reinterpret_cast<GraphicsManager*>(Engine::get().m_graphicsManager.get());
}

PhysicsManager& Engine::getPhysicsManager()
{
  return *reinterpret_cast<PhysicsManager*>(Engine::get().m_physicsManager.get());
}

EventManager& Engine::getEventManager()
{
  return *reinterpret_cast<EventManager*>(Engine::get().m_eventManager.get());
}

AudioManager& Engine::getAudioManager()
{
  return *reinterpret_cast<AudioManager*>(Engine::get().m_audioManager.get());
}


Engine& Engine::get()
{
  static Engine _instance;
  return _instance;
}

uint64_t Engine::addPlayerObject(const std::string& name)
{
  uint64_t objectId = generate_uuid();
  m_gameObjects[objectId] = std::make_shared<PlayerObject>(objectId, name);
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

void Engine::destroyObject(uint64_t id) {
  m_destroyQueue.push(id);
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
  uint64_t playerId = addPlayerObject("player");
  m_physicsManager->registerCollisionComponent(playerId, "box");
  m_physicsManager->registerPhysicsComponent(playerId, BodyType::Dynamic);
  // Tight hitbox — tune these to match the visible character body
  auto player = getGameObjectById(playerId).lock();
  if (player)
  {
    glm::vec2 frameSize = player->getTransform()->getScale();
    m_physicsManager->setColliderOffset(playerId, glm::vec2(0.0f, 0.0f));
    m_physicsManager->setColliderSize(playerId, glm::vec2(frameSize.x * 0.3f, frameSize.y * 0.45f));
  }
  m_eventManager->bindKey(GLFW_KEY_F1, GLFW_PRESS,
    []() {
      Engine::get().getPhysicsManager().toggleDebug();
    }
  );

  uint64_t platformId = addGameObject<GameObject>("platform", "assets/textures/blackBox.png");
  m_physicsManager->registerCollisionComponent(platformId, "box", true);
  m_physicsManager->registerPhysicsComponent(platformId, BodyType::Static);
  auto platform = Engine::get().getGameObjectById(platformId);
  if (auto platformPointer = platform.lock()) {
    platformPointer->getTransform()->setTranslation(glm::vec2(0.0f, -3.5f));
    platformPointer->getTransform()->setScale(glm::vec2(8.0f, 0.75f));
  }

  uint64_t smallPlatformId = addGameObject<GameObject>("platform", "assets/textures/blackBox.png");
  m_physicsManager->registerCollisionComponent(smallPlatformId, "box", true);
  m_physicsManager->registerPhysicsComponent(smallPlatformId, BodyType::Static);
  auto smallPlatform = Engine::get().getGameObjectById(smallPlatformId);
  if (auto smallPlatformPointer = smallPlatform.lock()) {
    smallPlatformPointer->getTransform()->setTranslation(glm::vec2(3.5f, -0.5f));
    smallPlatformPointer->getTransform()->setScale(glm::vec2(5.0f, 0.75f));
  }
}