#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "GraphicsManager.h"
#include "DisplayTypes.h"
#include "Shader.h"
#include "../Engine.h"
#include "../util.h"

void APIENTRY DebugCallback(GLenum source, GLenum type, GLuint id,
  GLenum severity, GLsizei length,
  const GLchar* message, const void* userParam)
{
  std::cerr << "GL ERROR: " << message << std::endl;
}

void HandleKeyInput(GLFWwindow* window, int key, int status, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
  else
  {
    DEBUG&& std::cout << "key: " << key << "; status: " << status << "; action: " << action << "; mods: " << mods << std::endl;
  }
}

GraphicsManager::GraphicsManager(int width, int height) : m_width(width), m_height(height), m_camera(std::make_shared<Camera>(width, height))
{
  if (!initGLFW()) {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    std::exit(-1);
  }

  glfwSetKeyCallback(m_window, HandleKeyInput);
  if (!initGLEW()) {
    std::cerr << "Failed to initialize GLEW" << std::endl;
    std::exit(-1);
  }

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

GraphicsManager::~GraphicsManager()
{
  glfwTerminate();
}

bool GraphicsManager::initGLFW()
{
  if (!glfwInit())
    return false;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef DEBUG
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

  m_window = glfwCreateWindow(m_width, m_height, "Grimlock", nullptr, nullptr);
  if (!m_window)
  {
    glfwTerminate();
    return false;
  }

  glfwMakeContextCurrent(m_window);

  return true;
}

bool GraphicsManager::initGLEW()
{
  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (err != GLEW_OK)
  {
    std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
    return false;
  }
  glGetError();

  return true;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void GraphicsManager::initialize()
{
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(DebugCallback, 0);
  int width, height;
  glfwGetFramebufferSize(m_window, &width, &height);
  glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);
}

void GraphicsManager::tick(float dt)
{
  if (glfwWindowShouldClose(m_window)) {
    Engine::get().killEngine();
  }
}

void GraphicsManager::prerender()
{
  glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GraphicsManager::render()
{
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void GraphicsManager::postrender()
{
  glfwSwapBuffers(m_window);
}

void GraphicsManager::setWindowHints()
{
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
  if (m_displayMode == DisplayType::BORDERLESS) {
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
  }
}

bool GraphicsManager::isRunning()
{
  return !glfwWindowShouldClose(m_window);
}

void GraphicsManager::shutdown()
{
  glfwDestroyWindow(m_window);
}

GLFWwindow* GraphicsManager::getWindow()
{
  return m_window;
}

std::shared_ptr<Camera> GraphicsManager::getCamera() const
{
  return m_camera;
}

std::shared_ptr<Shader> GraphicsManager::addShader(const std::string& vertexFile, const std::string& fragmentFile)
{
  std::shared_ptr<Shader> newShader = std::make_shared<Shader>(vertexFile, fragmentFile);
  m_shaders.push_back(newShader);
  return newShader;
}
