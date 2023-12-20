#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include "Graphics/Shader.h"
#include "Graphics/EBO.h"
#include "Graphics/VAO.h"
#include "Graphics/VBO.h"
#include "Graphics/GraphicsManager.h"
#include "Graphics/Texture.h"
#include "GameObject/Transform.h"

// Vertices coordinates
GLfloat vertices[] =
{ //     COORDINATES     /        COLORS      /   TexCoord  //
	-0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,	    0.0f, 0.0f, // Lower left corner
	-0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,	    0.0f, 1.0f, // Upper left corner
	 0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,	    1.0f, 1.0f, // Upper right corner
	 0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,	    1.0f, 0.0f  // Lower right corner
};

// Indices for vertices order
GLuint indices[] =
{
  0, 2, 1, // Upper triangle
  0, 3, 2 // Lower triangle
};

int main(void)
{
  GraphicsManager* graphicsManager = new GraphicsManager();
  Shader* shaderProgram = new Shader("assets/shaders/simple.vert", "assets/shaders/simple.frag");
  VAO vao1;
  vao1.bind();

  VBO vbo1(vertices, sizeof(vertices));
  EBO ebo1(indices, sizeof(indices));

  vao1.linkAttribute(vbo1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
  vao1.linkAttribute(vbo1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  vao1.linkAttribute(vbo1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  vao1.unbind();
  vbo1.unbind();
  ebo1.unbind();

  Transform* help = new Transform();
  help->setTranslation(glm::vec2(2, 2));
  help->setRotation(90);
  help->setScale(glm::vec2(2, 2));
  glm::mat3 result = help->calculateMatrix();
  
  Texture cool("assets/textures/cool.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
  cool.texUnit(*shaderProgram, "tex0", 0);

  graphicsManager->initialize();
  while (graphicsManager->isRunning())
  {
    vao1.bind();
    shaderProgram->activate();
    cool.bind();
    graphicsManager->tick(0);
  }

  vao1.shutdown();
  vbo1.shutdown();
  ebo1.shutdown();
  cool.shutdown();
  shaderProgram->shutdown();
  graphicsManager->shutdown();
  delete shaderProgram;
  delete graphicsManager;
  delete help;
  return 0;
}