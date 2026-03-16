#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include "Engine.h"
#include "Test/CrashHandler.h"

int main(void)
{
  installCrashHandler();

  Engine& gameEngine = Engine::get();

  gameEngine.testCase1();
  gameEngine.initialize();
  while (gameEngine.gameIsRunning())
  {
    gameEngine.gameLoop();
  }

  gameEngine.shutdown();
  return 0;
}