#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Engine.h"
#include "GameObject/GameObject.h"

int main(void)
{
  Engine& gameEngine = Engine::get();
  GameObject* gameObject = new GameObject();

  gameEngine.initialize();
  gameObject->initialize();
  while (gameEngine.gameIsRunning())
  {
    gameEngine.gameLoop();
    float dt = gameEngine.getDeltaTime();
    gameObject->tick(dt);
    gameObject->render();
  }

  gameEngine.shutdown();
  gameObject->shutdown();
  delete gameObject;
  return 0;
}