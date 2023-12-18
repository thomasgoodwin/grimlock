#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "./Graphics/Shader.h"
#include "./Graphics/EBO.h"
#include "./Graphics/VAO.h"
#include "./Graphics/VBO.h"
#include "./Graphics/GraphicsManager.h"

GLfloat vertices[] =
    {
        -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,    // Lower left corner
        0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,     // Lower right corner
        0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f,  // Upper corner
        -0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner left
        0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,  // Inner right
        0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f      // Inner down
};

GLuint indices[] =
    {
        0, 3, 5,
        3, 2, 4,
        5, 4, 1};


int main(void)
{
  GraphicsManager* graphicsManager = new GraphicsManager();
  Shader* shaderProgram = new Shader("assets/shaders/simple.vert", "assets/shaders/simple.frag");
  VAO vao1;
  vao1.bind();

  VBO vbo1(vertices, sizeof(vertices));
  EBO ebo1(indices, sizeof(indices));

  vao1.linkVBO(vbo1, 0);
  vao1.unbind();
  vbo1.unbind();
  ebo1.unbind();

  graphicsManager->initialize();
  while (graphicsManager->isRunning())
  {
    vao1.bind();
    shaderProgram->activate();
    graphicsManager->tick(0);
  }

  vao1.shutdown();
  vbo1.shutdown();
  ebo1.shutdown();
  shaderProgram->shutdown();
  graphicsManager->shutdown();
  delete shaderProgram;
  delete graphicsManager;
  
  return 0;
}