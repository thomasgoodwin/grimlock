#ifndef GRAPHICS_MANAGER_H
#define GRAPHICS_MANAGER_H
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>
#include "DisplayTypes.h"

class Shader;

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
    std::shared_ptr<Shader> addShader(const std::string &vertexFile, const std::string &fragmentFile);
    GLFWwindow* getWindow();

  private:
    void SetWindowHints();
    GLFWwindow* window;
    DisplayType m_displayMode = DisplayType::BORDERLESS;
    std::vector<std::shared_ptr<Shader>> m_shaders;
};

#endif