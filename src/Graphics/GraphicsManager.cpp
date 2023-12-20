#include <GL/glew.h>
#include <iostream>
#include "GraphicsManager.h"
#include "../util.h"

void HandleKeyInput(GLFWwindow* window, int key, int status, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
  else
  {
    DEBUG && std::cout << "key: " << key << "; status: " << status << "; action: " << action << "; mods: " << mods << std::endl;
  }
}

GraphicsManager::GraphicsManager()
{
  if (!glfwInit())
  {
    std::cerr << "glfw init failed" << std::endl;
  }

  SetWindowHints();
  window = glfwCreateWindow(1280, 720, "Grimlock", nullptr, nullptr);
  if (window == nullptr)
  {
    std::cerr << "GLFW window creation failed." << std::endl;
    glfwTerminate();
  }

  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, HandleKeyInput);
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK)
  {
    std::cout << "GLEW init failed." << std::endl;
    glfwDestroyWindow(window);
    glfwTerminate();
  }
}

GraphicsManager::~GraphicsManager() 
{
  glfwTerminate();
}
void GraphicsManager::initialize()
{
  glClearColor(0.15f, 0.15f, 0.2f, 1.0f);
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, 800, 800);
}

void GraphicsManager::tick(float dt)
{
  glfwPollEvents();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  glfwSwapBuffers(window);
}

void GraphicsManager::SetWindowHints()
{
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
}

bool GraphicsManager::isRunning()
{
  return !glfwWindowShouldClose(window);
}

void GraphicsManager::shutdown()
{
  glfwDestroyWindow(window);
}

GLFWwindow* GraphicsManager::getWindow()
{
  return window;
}