#include <iostream>

#include "../glew/include/GL/glew.h"
#include "../glfw/include/GLFW/glfw3.h"

const bool DEBUG = false;

void setWindowHints()
{
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
}

void handleKeyInput(GLFWwindow *window, int key, int status, int action, int mods)
{
  if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
  else {
    DEBUG && std::cout << "key: " << key << "; status: " << status << "; action: " << action << "; mods: " << mods << std::endl;
  }
}

int main(void)
{
  if (!glfwInit())
  {
    std::cerr << "glfw init failed" << std::endl;
    return -1;
  }
  setWindowHints();
  GLFWwindow *window = glfwCreateWindow(1280, 720, "Grimlock", nullptr, nullptr);
  if (window == nullptr)
  {
    std::cerr << "GLFW window creation failed." << std::endl;
    glfwTerminate();
    return -2;
  }
  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, handleKeyInput);

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK)
  {
    std::cout << "GLEW init failed." << std::endl;
    glfwDestroyWindow(window);
    glfwTerminate();
    return -3;
  }

  glClearColor(0.15f, 0.15f, 0.2f, 1.0f);
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