#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

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
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
  else
  {
    DEBUG &&std::cout << "key: " << key << "; status: " << status << "; action: " << action << "; mods: " << mods << std::endl;
  }
}

float vertices[] = {
    -0.5f,
    -0.5f,
    0.0f,
    0.5f,
    -0.5f,
    0.0f,
    0.0f,
    0.5f,
    0.0f,
};

std::string readFile(const std::string &fileLoc)
{
  std::ifstream file;
  file.open(fileLoc, std::ios::in);
  std::stringstream contents;
  if (!file.fail())
  {
    contents << file.rdbuf();
  }
  else
  {
    std::cerr << "Failed to read file: " << fileLoc << std::endl;
  }
  return contents.str();
}

GLuint createAndCompileShader(const std::string &fileLoc, GLuint shaderType)
{
  const std::string shaderSource = readFile(fileLoc);
  const char *cShader = shaderSource.c_str();
  GLuint shader = glCreateShader(shaderType);

  glShaderSource(shader, 1, &cShader, nullptr);
  glCompileShader(shader);

  int success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    char infoLog[512];
    glGetShaderInfoLog(shader, 512, nullptr, infoLog);
    std::cerr << "gl shader compilation error: " << infoLog << std::endl;
  }

  return shader;
}

GLuint createAndLinkProgram()
{
  GLuint vertexShader = createAndCompileShader("../shaders/simple.vert", GL_VERTEX_SHADER);
  GLuint fragmentShader = createAndCompileShader("../shaders/simple.frag", GL_FRAGMENT_SHADER);
  GLuint program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);

  int success;
  glGetProgramiv(program, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    char infoLog[512];
    glGetProgramInfoLog(program, 512, nullptr, infoLog);
    std::cerr << "gl program link error: " << infoLog << std::endl;
  }

  return program;
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

  GLuint shaderProgram = createAndLinkProgram();

  GLuint VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  glClearColor(0.15f, 0.15f, 0.2f, 1.0f);
  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}