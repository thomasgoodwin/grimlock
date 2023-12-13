#include <iostream>

#include "GL/glew.h"
#include "glfw/glfw3.h"

int main(void)
{
  if (!glfwInit())
  {
    std::cerr << "glfw init failed" << std::endl;
    return -1;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
  GLFWwindow *window = glfwCreateWindow(1280, 720, "Grimlock", nullptr, nullptr);
  if (window == nullptr)
  {
    std::cerr << "GLFW window creation failed." << std::endl;
    glfwTerminate();
    return -2;
  }
  glfwMakeContextCurrent(window);
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK)
  {
    std::cout << "GLEW init failed." << std::endl;
    glfwDestroyWindow(window);
    glfwTerminate();
    return -3;
  }

  glClearColor(0.3f, 0.3f, 0.4f, 1.0f);
  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwSwapBuffers(window);
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}