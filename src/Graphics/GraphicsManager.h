#ifndef GRAPHICS_MANAGER_H
#define GRAPHICS_MANAGER_H
#include <vector>
#include <memory>
#include <string>
#include "DisplayTypes.h"
#include "Camera/Camera.h"

class Shader;
struct GLFWwindow;

class GraphicsManager
{
public:
  GraphicsManager();
  ~GraphicsManager();
  bool initGLFW();
  bool initGLEW();
  void updateViewport();
  void initialize();
  void tick(float dt);
  void prerender();
  void render();
  void postrender();
  void shutdown();
  bool isRunning();
  std::shared_ptr<Shader> addShader(const std::string& vertexFile, const std::string& fragmentFile);
  GLFWwindow* getWindow();
  std::shared_ptr<Camera> getCamera() const;
  int getWidth() const;
  int getHeight()const;
  float getAspectRatio() const;
private:
  void setWindowHints();
  int m_width;
  int m_height;
  GLFWwindow* m_window;
  DisplayType m_displayMode = DisplayType::BORDERLESS;
  std::vector<std::shared_ptr<Shader>> m_shaders;
  std::shared_ptr<Camera> m_camera;
  float m_aspectRatio;
};

#endif