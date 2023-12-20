#ifndef GRAPHICS_MANAGER_H
#define GRAPHICS_MANAGER_H
#include <GLFW/glfw3.h>
#include "DisplayTypes.h"

class GraphicsManager
{
  public:
    GraphicsManager();
    ~GraphicsManager();
    void initialize();
    void tick(float dt);
    void render();
    void shutdown();
    bool isRunning();
    GLFWwindow* getWindow();
  private:
    void SetWindowHints();
    GLFWwindow* window;
    DisplayType m_displayMode = DisplayType::BORDERLESS;
};

#endif