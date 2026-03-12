#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include "Engine.h"

int main(void)
{
  std::cout << "HERE" << std::endl;
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